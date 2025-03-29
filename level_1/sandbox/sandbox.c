#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid < 0)
        return -1;
    if (pid == 0)
    {
        alarm(timeout);
        f();
        exit(0);
    }
    if (waitpid(pid, &status, 0) == -1)
        return -1;
    if (WIFEXITED(status))
    {
        if (WEXITSTATUS(status) == 0)
        {
            if (verbose)
                printf("Nice function!\n");
            return 1;
        }
        if (verbose)
            printf("Bad function: exited with code %d\n", WEXITSTATUS(status));
        return 0;
    }
    if (WIFSIGNALED(status))
    {
        if (verbose)
            printf("Bad function: %s\n", strsignal(WTERMSIG(status)));
        return 0;
    }
    return 0;
}


void fun1() {
    printf("alo\n");
    sleep(1);
    printf("alo2\n");
    sleep(1);
    printf("alo3\n");
    sleep(1);
}

void fun2() {
    exit(42);
}

void fun3() {
    int *p = NULL;
    *p = 42;
}

int main()
{
    sandbox(fun1, 2, 1); // Test function that segfaults
    sandbox(fun2, 5, 1);  // Test function that segfaults
    sandbox(fun3, 5, 1);  // Test function that segfaults
    while(1)
    {

    }
}
