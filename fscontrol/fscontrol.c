#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "fscontrol.h"

void fperms (char* path, char* mode) {
    long perms = strtol(mode, 0, 8); // Convert permission string to long
    if(chmod(path, perms) < 0) { // Chmod system call to update file permissions
        printf("Error: %s\n", strerror(errno));
    } else {
        printf("Updated permissions of %s to %s (%ld)\n", path, mode, perms);
    }
}
