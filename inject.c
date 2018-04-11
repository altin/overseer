#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

const char add_group[] = "groupadd overseen";
const char cp_script[] = "cp overseen-ug.sh /etc";
const char run_script[] = "sh ./overseen-ug.sh";
FILE* profile;

void install() {
    system(add_group);
    system(cp_script); // Copy shell script to /etc
    profile = fopen("/etc/profile", "a");
    fprintf(profile, "%s", run_script); // Append shell execution command to /etc/profile
    fclose(profile);
}

void uninstall() {
    // TODO(randy/altin): Error handling.
    profile = fopen("/etc/profile", "r");
    FILE* temp = fopen("/etc/profile_temp", "w");
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), profile)) {
        if (strcmp(buffer, run_script) == 0) {
            continue;
        } else if (strcmp(buffer, "\n") == 0) {
            strcat(buffer, "");
        }
        fputs(buffer, temp);
        printf("Add: %s", buffer);
    }

    fclose(profile);
    fclose(temp);

    remove("/etc/profile");
    rename("/etc/profile_temp", "/etc/profile");
}

int main(int argc, char** argv) {
    if (strcmp(argv[1], "install") == 0) {
        install();
    } else if (strcmp(argv[1], "uninstall") == 0) {
        uninstall();
    }

    return 0;
}
