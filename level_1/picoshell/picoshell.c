#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int picoshell(char **cmds[])
{
    pid_t pid;
    int fd[2];
    int prev_fd = -1;
    int i = 0;
    int status;

    while(cmds[i])
    {
        if (cmds[i + 1])
        {
            if (pipe(fd) == -1)
                return 1;
        }
        pid = fork();
        if (pid < 0)
        {
            if (prev_fd != -1)
                close(prev_fd);
            if (cmds[i + 1])
            {
                close(fd[0]);
                close(fd[1]);
            }
            return 1;
        }
        if (pid == 0)
        {
            if (prev_fd != -1)
            {
                if (dup2(prev_fd, 0) == -1)
                    exit (1);
                close(prev_fd);
            }
            if (cmds[i + 1])
            {
                if (dup2(fd[1], 1) == -1)
                    exit (1);
                close(fd[1]);
                close(fd[0]);
            }
            if (execvp(cmds[i][0], cmds[i]) == -1)
                exit(1);
        }
        if (prev_fd != -1)
            close(prev_fd);
        if (cmds[i + 1])
        {
            close(fd[1]);
            prev_fd = fd[0];
        }
        i++;
    }
    if (prev_fd != -1)
        close(prev_fd);
    while(wait(&status) > 0);
    return 0;
}

int main()
{
    char *cmd1[] = {"echo", "alooo", NULL};
    char *cmd2[] = {"ls", NULL};
    char **cmds[] = {cmd1, cmd2, NULL};
    picoshell(cmds);
    while(1){}
}