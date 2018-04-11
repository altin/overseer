#include "overseer.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>
#include <sys/stat.h>

const char add_group[] = "groupadd overseen";
const char cp_script[] = "cp overseen-ug.sh /etc";
const char run_script[] = "bash /etc/overseen-ug.sh";
FILE* profile;

void install() {
    system(add_group);
    system(cp_script); // Copy shell script to /etc

    profile = fopen("/etc/profile", "a");
    if (profile == NULL) {
        printf("Error opening /etc/profile.\n");
        return;
    }

    fprintf(profile, "%s", run_script); // Append shell execution command to /etc/profile
    fclose(profile);
    printf("Installed overseer under user group 'overseen'\n");
}

void uninstall() {
    profile = fopen("/etc/profile", "r");
    if (profile == NULL) {
        printf("Error opening /etc/profile.\n");
        return;
    }

    FILE* temp = fopen("/etc/profile_temp", "w");
    if (temp == NULL) {
        printf("Error opening /etc/profile_temp.\n");
        return;
    }

    char buffer[256];

    while (fgets(buffer, sizeof(buffer), profile)) {
        if (strcmp(buffer, run_script) == 0) {
            continue;
        } else if (strcmp(buffer, "\n") == 0) {
            strcat(buffer, "");
        }
        fputs(buffer, temp);
    }

    fclose(profile);
    fclose(temp);

    remove("/etc/profile");
    rename("/etc/profile_temp", "/etc/profile");
    delete_group();
}

void delete_group() {
    char buffer[256];
    char* gid;
    char* user;
    // struct group *grp;
    struct passwd* pwd;
    //char* p = calloc()

    // Find GID of overseer group name
    profile = fopen("/etc/group", "r");
    while (fgets(buffer, sizeof(buffer), profile)) {
        gid = strstr(buffer, "overseen:x");
        if (gid == NULL) {
            continue;
        } else {
            gid = strtok(buffer, "overseen:x\n");
        }
    }

    // Find user with overseen GID
    profile = fopen("/etc/passwd", "r");

    char delim[256];
    char delim2[256];
    sprintf(delim, ":%s:", gid);
    sprintf(delim2, ":%s:\n", gid);

    setpwent();
    char command[256];

    while (pwd) {
        if (pwd->pw_gid == atoi(gid)) {
            printf("%s", pwd->pw_name);
            sprintf(command, "usermod -g users %s", pwd->pw_name);
            system(command);

            printf("Kicked %s out of overseen\n", pwd->pw_name);
        }
        pwd = getpwent();
    }
    endpwent();

    system("delgroup overseen");

    /*
    while (fgets(buffer, sizeof(buffer), profile)) {
        user = strstr(buffer, delim);
        if (user == NULL) {
            continue;
        } else {
            user = strtok(buffer, delim2);
            printf("%s", user);

            char command[256];
            sprintf(command, "usermod -g users %s", user);
            system(command);

            printf("Kicked %s out of overseen\n", user);

        }

        //system("delgroup overseen");
    }
    */

    //gid_t man = 1002;
    //grp = getgrgid(man);
    //users = grp->gr_mem;
    //char** ptr = grp->gr_mem;
    //int i =0;
    //ptr[0] = "bob";
    //printf("%s", *ptr++);
    //printf("%s", *users[0]);
    //while (*ptr != NULL) {
        //printf("%s", *ptr);
        //ptr++;
    //}
    //for (char* c = *ptr; c; c=*++ptr) {
        //printf("%s", c);
    //}
    //for (char** p = users; *p; ++p) {
        //char* temp = *p;
        //printf("%s", temp);
    //}
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Only install or uninstall arguments are permitted.\n");
        return -1;
    }

    if (strcmp(argv[1], "install") == 0) {
        install();
    } else if (strcmp(argv[1], "uninstall") == 0) {
        uninstall();
    } else if (strcmp(argv[1], "group") == 0) {
        delete_group();
    } else {
        printf("Invalid parameter \"%s\" specified.\n", argv[1]);
        printf("Only install or uninstall arguments are permitted.\n");
        return -1;
    }

    return 0;
}