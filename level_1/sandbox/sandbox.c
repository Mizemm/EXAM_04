#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>


int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
    pid_t pid;
    int status;
    int exit_code;
    int signal;
    unsigned int time = 0;

    pid = fork();
    if (pid < 0)
        return -1;
    if (pid == 0)
    {
        alarm(timeout);
        f();
        exit(0);
    }
    while(waitpid(pid, &status, WNOHANG) == 0)
    {
        sleep(1);
        time++;
        if (time >= timeout)
        {
            kill(pid, SIGKILL);
            waitpid(pid, &status, 0);
            if (verbose)
                printf("Bad function: timed out after %u seconds\n", timeout);   
            return 0;
        }
    }
    if (WIFEXITED(status))
    {
        exit_code = WEXITSTATUS(status);
        if (exit_code == 0)
        {
            if (verbose)
                printf("Nice function!\n");
            return 1;
        }
        else
        {
            if (verbose)
                printf("Bad function: exited with code %d\n", exit_code);
            return 0;
        }
    }
    if (WIFSIGNALED(status))
    {
        signal = WTERMSIG(status);
        if (verbose)
            printf("Bad function: %s\n", strsignal(signal));
    }
    return 0;
}

// void fun1()
// {
//     printf("alo\n");
//     sleep(5);
//     printf("alo2\n");
//     sleep(5);
//     printf("alo3\n");
//     sleep(4);

// }
// void fun2()
// {
//     exit(42);
// }
// void fun3()
// {
//     int *p = NULL;
//     *p = 42;
// }
// int main()
// {
//     sandbox(fun3, 5, 1);
// }