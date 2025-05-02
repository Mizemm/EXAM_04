#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


void nothing(int sig)
{
    (void)sig;
}

int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
    pid_t pid =  fork();
    if (pid == -1)
        return -1;
    if (pid == 0)
    {
        alarm(timeout);
        f();
        exit(0);
    }

    struct sigaction sa;
    sa.sa_handler = nothing;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGALRM, &sa, NULL);


    alarm(timeout);
    int st;
    pid_t r = waitpid(pid, &st, 0);
    if (r == -1)
    {
        if (errno == EINTR)
        {
            kill(pid, SIGKILL);
            waitpid(pid, NULL, 0);
            if (verbose)
                printf("timeout");
            return 0;
        }
        return -1;
    }
    if (WIFEXITED(st))
    {
        if (WEXITSTATUS(st) == 0)
        {
            if (verbose)
                printf("good");
            return 1;
        }
        if (verbose)    
            printf("bad exit");
        return 0;
    }
    if (WIFSIGNALED(st))
    {
        if (WTERMSIG(st) == SIGALRM)
        {
            if (verbose)
                printf("trrr");
            return 0;
        }
        if (verbose)
            printf("%s", strsignal(WTERMSIG(st)));
        return 0;
    }
    return -1;
}

void fun1() {
    printf("alo\n");
    sleep(2);
    // printf("alo2\n");
    // sleep(1);
//     printf("alo3\n");
//     sleep(1);
}

void fun2() {
    exit(42);
}

void fun3() {
    int *p = NULL;
    *p = 42;
}

void fun0() {
	printf("wow\n");
}

int main()
{

	printf("%d\n", sandbox(fun0, 1, 1));

    // sandbox(fun1, 1, 1);

    // sandbox(fun2, 5, 1);

    // sandbox(fun3, 5, 1);
    // while(1)
    // {

    // }
}