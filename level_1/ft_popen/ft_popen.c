#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int	ft_popen(const char *file, char *const argv[], char type)
{

    pid_t pid;
    int fd[2];
    
    if (!file || !argv || (type != 'r' && type != 'w'))
        return -1;
    if (pipe(fd) == -1)
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
            if (dup2(fd[1], 0) == -1)
                exit (-1);
        }
        else if (type == 'w')
        {
            if (dup2(fd[0], 0) == (-1))
                exit (-1);
        }
        close(fd[0]);
        close(fd[1]);
        if (execvp(file, argv) == -1)
            exit (-1);
    }
    if (type == 'r')
    {
        close(fd[1]);
        return(fd[0]);
    }
    else if (type == 'w')
    {
        close(fd[0]);
        return (fd[1]);
    }
    return 0;
}
int main() 
{
    printf("fd : %d\n", ft_popen("ls", (char *const[]){"ls", NULL}, 'r'));
}