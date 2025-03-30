#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int ft_popen(const char *file, const char *argv[], char type)
{
    if (!file || !argv || (type != 'r' && type != 'w'))
        return -1;

    int fd[2];
    if (pipe(fd) == -1)
        return -1;

    pid_t pid = fork();
    if (pid == -1) {
        close(fd[0]);
        close(fd[1]);
        return -1;
    }
    if (pid == 0) {  // Child process
        if (type == 'r') {
            close(fd[0]);          // Close unused read end
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
        } else {  // type == 'w'
            close(fd[1]);          // Close unused write end
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
        }
        execvp(file, (char *const *)argv);
        exit(-1);  // Only reached if execvp fails
    }
    if (type == 'r') {
        close(fd[1]);  // Close write end in parent
        return fd[0];  // Return read end
    } else {  // type == 'w'
        close(fd[0]);  // Close read end in parent
        return fd[1];  // Return write end
    }
}

int main() 
{
    printf("fd : %d\n", ft_popen("ls", (char *const[]){"ls", NULL}, 'r'));
    printf("fd : %d\n", ft_popen("ls", (char *const[]){"ls", NULL}, 'w'));
    while(1){}
}