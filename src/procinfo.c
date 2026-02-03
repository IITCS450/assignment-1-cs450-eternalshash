#include "common.h"
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static void usage(const char *a){fprintf(stderr,"Usage: %s <pid>\n",a); exit(1);}
static int isnum(const char *s){for(;*s;s++) if(!isdigit(*s)) return 0; return 1;}

int main(int c, char **v){
    if(c!=2 || !isnum(v[1])) usage(v[0]);

    char *pid = v[1];
    char path[256], line[1024];
    FILE *fp;

    sprintf(path, "/proc/%s/status", pid);
    fp = fopen(path, "r");
    if(!fp) { perror("Error"); return 1; } // pid working / or not 
    char state[256] = "Unknown";
    char rss[256] = "0";
    while(fgets(line, sizeof(line), fp)) {
        if(strncmp(line, "State:", 6) == 0) strcpy(state, line + 7);
        if(strncmp(line, "VmRSS:", 6) == 0) strcpy(rss, line + 7);
    }
    fclose(fp);

    sprintf(path, "/proc/%s/stat", pid);
    fp = fopen(path, "r");
    int ppid = 0;
    unsigned long utime = 0, stime = 0;
    if(fp) {
        // CHECK THIS AGAIN
        fscanf(fp, "%*d %*s %*c %d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu", &ppid, &utime, &stime);
        fclose(fp);
    }

    sprintf(path, "/proc/%s/cmdline", pid);
    fp = fopen(path, "r");
    char cmd[1024] = "";
    if(fp) {
        fgets(cmd, sizeof(cmd), fp);
        fclose(fp);
    }


    printf("Process state: %s", state);
    printf("Parent PID: %d\n", ppid);
    printf("Command line: %s\n", cmd);
    printf("CPU time: user=%lu, system=%lu\n", utime, stime);
    printf("Resident memory usage: %s", rss);

    return 0;
}

