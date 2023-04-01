#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// #include <sys/wait.h>

int main() {
    int a, b, status;
    pid_t pid;

    printf("Enter the values of a and b in the equation ax + b = 0: ");
    scanf("%d %d", &a, &b);

    pid = fork();

    if (pid == 0) {
        // child process (P2)
        int result = -b/a;
        exit(result);
    } else if (pid > 0) {
        // parent process (P1)
        waitpid(pid, &status, 0);
        int exit_status = WEXITSTATUS(status);
        printf("Child process PID: %d\n", pid);
        printf("Child process exit status: %d\n", exit_status);
    } else {
        // fork error
        fprintf(stderr, "Fork failed.\n");
        return 1;
    }

    return 0;
}
