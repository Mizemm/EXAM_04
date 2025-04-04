// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <sys/wait.h>

// int picoshell(char **cmds[])
// {
//     pid_t pid;
//     int fd[2];
//     int prev_fd = -1;
//     int i = 0;
//     int status;

//     while(cmds[i])
//     {
//         if (cmds[i + 1])
//         {
//             if (pipe(fd) == -1)
//                 return 1;
//         }
//         pid = fork();
//         if (pid < 0)
//         {
//             if (prev_fd != -1)
//                 close(prev_fd);
//             if (cmds[i + 1])
//             {
//                 close(fd[0]);
//                 close(fd[1]);
//             }
//             return 1;
//         }
//         if (pid == 0)
//         {
//             if (prev_fd != -1)
//             {
//                 if (dup2(prev_fd, 0) == -1)
//                     exit (1);
//                 close(prev_fd);
//             }
//             if (cmds[i + 1])
//             {
//                 if (dup2(fd[1], 1) == -1)
//                     exit (1);
//                 close(fd[1]);
//                 close(fd[0]);
//             }
//             if (execvp(cmds[i][0], cmds[i]) == -1)
//                 exit(1);
//         }
//         if (prev_fd != -1)
//             close(prev_fd);
//         if (cmds[i + 1])
//         {
//             close(fd[1]);
//             prev_fd = fd[0];
//         }
//         i++;
//     }
//     if (prev_fd != -1)
//         close(prev_fd);
//     while(wait(&status) > 0);
//     return 0;
// }

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void piping(char ***cmds, int index, int *res, int saved_stdin, int saved_stdout)
{
	int fd[2];

	pipe(fd);
	if (cmds[index + 1])
		dup2(fd[1], STDOUT_FILENO);
	if (fork() == 0)
	{
		close(fd[0]);
		close(fd[1]);
		close(saved_stdin);
		close(saved_stdout);
		execvp(cmds[index][0], cmds[index]);
		exit(1);
	}
	dup2(fd[0], STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	if (cmds[index + 1] == NULL)
	{
		dup2(saved_stdin, STDIN_FILENO);
		int status;
		while (wait(&status) != -1)
		{
			if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
                *res = 1;
		}
		return ;
	}
	piping(cmds, index + 1, res, saved_stdin, saved_stdout);
}

int picoshell(char **cmds[])
{
	int saved_stdin = dup2(STDIN_FILENO, 1000);
	int saved_stdout = dup2(STDOUT_FILENO, 1001);
	int res = 0;

	piping(cmds, 0, &res, saved_stdin, saved_stdout);

	close(saved_stdin);
	close(saved_stdout);

	return (res);
}

int main()
{
    char *cmd1[] = {"echo", "alooo", NULL};
    char *cmd2[] = {"ls", NULL};
    char **cmds[] = {cmd1, cmd2, NULL};
    picoshell(cmds);
    while(1){}
}