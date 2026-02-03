#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [args...]\n", argv[0]);
        return 1;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    pid_t pid = fork(); // wasn working 

    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        
        execvp(argv[1], &argv[1]);
        perror("execvp failed");
        exit(1);
    } else {
        
        int status;
        waitpid(pid, &status, 0);
        
        clock_gettime(CLOCK_MONOTONIC, &end);

        // wall clock 
        double elapsed = (end.tv_sec - start.tv_sec) + 
                         (end.tv_nsec - start.tv_nsec) / 1000000000.0;

        int exit_val = WIFEXITED(status) ? WEXITSTATUS(status) : WTERMSIG(status);
        
        printf("pid=%d elapsed=%.3f exit=%d\n", pid, elapsed, exit_val);
    }

    return 0;
}
