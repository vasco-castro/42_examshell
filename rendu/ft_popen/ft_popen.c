
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int ft_popen(const char *file, char *const argv[], char type)
{
	int fd[2];
	int pid;
	int io = 0;

	if (!file || !argv || (type != 'r' && type != 'w'))
		return -1;
	if (type == 'w')
		io = 1;
	if (pipe(fd) == -1)
		return -1;
	pid = fork();
	if (pid == -1)
		return (close(fd[io]), close(fd[!io]), -1);
	if (pid == 0)
	{
		dup2(fd[io], io);
		close(fd[io]);
		close(fd[!io]);
		execvp(file, argv);
		exit(1);
	}
	return (close(fd[io]), fd[!io]);
}
