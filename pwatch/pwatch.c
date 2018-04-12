#include <ctype.h>
#include <dirent.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>

#include "../third_party/frozen/frozen.c"

#define MAX_NAME_LEN 256

int kill_process(const char*);
static void scan_array(const char*, int, void*);
void cleanup();

typedef struct Process {
    char* name;
} Process;

Process* processes;
static unsigned int num_processes = 0;

int main(int argc, char** argv) {
    char* config_json = json_fread("../config.json");
    if (config_json == NULL) {
        printf("Failed to read \"config.json\"");
        return -1;
    }

    // How long to sleep in the kill loop below.
    // Default value is 1 second.
    int process_kill_wait_time = 1;
    json_scanf( config_json
              , strlen(config_json)
              , "{ process_kill_wait_time:%d, blacklisted_processes:%M }"
              , &process_kill_wait_time
              , scan_array
              );
    free(config_json);

    struct sysinfo si;
    int num_sys_processes = sysinfo(&si) == 0 ? (int)si.procs : 0;

    while (1) {
        sleep(process_kill_wait_time);
        if (sysinfo(&si) == 0) {
            // Check if a new process has started.
            if (num_sys_processes != (int)si.procs) {
                // Check if the new process that was started is in our blacklist.
                // If it is, we kill it.
                for (unsigned int i = 0; i < num_processes; i++) {
                    kill_process(processes[i].name);
                }
                num_sys_processes = (int)si.procs;
            }
        }
    }

    cleanup();

    return 0;
}

static void scan_array(const char* str, int len, void* user_data) {
    struct json_token t;
    // Parse the array to determine its length.
    for (int i = 0; json_scanf_array_elem(str, len, "", i, &t) > 0; i++) {
        num_processes++;
    }

    processes = malloc(num_processes * sizeof(*processes));
    for (int i = 0; json_scanf_array_elem(str, len, "", i, &t) > 0; i++) {
        char process_name[MAX_NAME_LEN];
        sprintf(process_name, "%.*s", t.len, t.ptr);
        processes[i].name = (char*)malloc(strlen(process_name) + 1);
        strcpy(processes[i].name, process_name);
    }
}

int kill_process(const char* name) {
    DIR* dir = opendir("/proc");

    if (dir == NULL) {
        printf("Failed to open /proc directory\n");
        return -1;
    }
    struct dirent* ent;

    unsigned short skip_process = 0;

    // Iterate through each folder in the directory.
    while ((ent = readdir(dir)) != NULL) {
        // Skip folders that aren't processes (if they contain characters
        // that aren't numbers).
        for (unsigned int i =0; i < strlen(ent->d_name); i++) {
            if (!isdigit(ent->d_name[i])) {
                skip_process = 1;
                break;
            }
        }
        if (skip_process) {
            skip_process = 0;
            continue;
        }

        // Determine the path of the process.
        char s[1000];
        strcpy(s, "/proc/");
        strcat(s, ent->d_name);
        strcat(s, "/comm");

        FILE* f = fopen(s, "r");
        if (f == NULL) {
            // printf("Failed to open file \"%s\".\n", s);
            // closedir(dir);
            // return -1;
            continue;
        }

        char buffer[MAX_NAME_LEN];
        // Copy the contents of the file into buffer.
        while (fgets(buffer, sizeof(buffer), f)) {}
        fclose(f);

        // Strip the newline character from the buffer.
        strtok(buffer, "\n");
        // If name exists in buffer, kill the corresponding process.
        if (strstr(name, buffer)) {
            // We do not need to do any error handling here, because the for loop above
            // determined whether or not the directory name consists of numbers.
            pid_t p = atoi(ent->d_name);
            kill(p, SIGKILL);
        }
    }
    closedir(dir);

    return 0;
}

void cleanup() {
    for (unsigned int i = 0; i < num_processes; i++) {
        free(processes[i].name);
        processes[i].name = NULL;
    }
    free(processes);
    processes = NULL;
}
