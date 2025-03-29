#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int ft_popen(const char *file, char *const argv[], char type)
{
    pid_t pid;
    int fd[2];

    if (!file || !argv || (type != 'r' && type != 'w') || pipe(fd) == -1)
        return -1;
    pid = fork();
    if (pid < 0)
    {
        close(fd[0]);
        close(fd[1]);
        return -1;
    }
    if (pid == 0)
    {
        if (type == 'r')
        {
            close(fd[0]);
            if (dup2(fd[1], 1) == -1)
                exit(-1);
        }
        else if (type == 'w')
        {
            close(fd[1]);
            if (dup2(fd[0], 0) == -1)
                exit(-1);
        }
        close(fd[0]);
        close(fd[1]);
        if (execvp(file, argv) == -1)
            exit(-1);
    }
    int tmp_fd;
    if (type == 'r') 
    {
        close(fd[1]);
        tmp_fd = fd[0];
        close (fd[0]);
        return tmp_fd;
    }
    else if (type == 'w') 
    {
        close(fd[0]);
        tmp_fd = fd[1];
        close(fd[1]);
        return tmp_fd;
    }
    return 0;
}
int main() 
{
    printf("fd : %d\n", ft_popen("ls", (char *const[]){"ls", NULL}, 'r'));
    printf("fd : %d\n", ft_popen("ls", (char *const[]){"ls", NULL}, 'w'));
    while(1){}
}