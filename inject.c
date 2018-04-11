#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

const char add_group[] = "groupadd overseen";
const char cp_script[] = "cp overseen-ug.sh /etc";
const char run_script[] = "sh ./overseen-ug.sh";
FILE* profile;

void install() {
    system(add_group); // Create group
    system(cp_script); // Copy shell script to /etc
    profile = fopen("/etc/profile", "a"); // Open /etc/profile in append mode
    fprintf(profile, "%s", run_script); // Append shell execution command to /etc/profile
    fclose(profile);
}

void uninstall() {
    profile = fopen("/etc/profile", "r+"); // Open /etc/profile in read/write mode
    char buffer [256];
    while (fgets(buffer, sizeof(buffer), profile)) {
        if (strcmp(buffer, run_script) != 0) {
            fputs(buffer, profile);
        }
        printf("Add: %s", buffer);
    }
    fclose(profile);
}

int main(int argc, char** argv) {
    if (strcmp(argv[1], "install") == 0) {
        install();
    }
    else if (strcmp(argv[1], "uninstall") == 0) {
        uninstall();
    }
    return 0;
}