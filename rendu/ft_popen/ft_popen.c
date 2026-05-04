#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int	ft_popen(const char *file, char *const argv[], char type)
{
	int fd[2];
	int pid;
	int io = 0;

	if (type == 'r')
		io = 1;
	if (!file || !argv || (type != 'r' && type != 'w'))
		return (-1);
	if (pipe(fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (close(fd[0]), close(fd[1]), -1);
	if (pid == 0)
	{
		puts("child");
		dup2(fd[io], io);
		close(fd[0]);
		close(fd[1]);
		execvp(file, argv);
		exit(1);
	}
	puts("psar");
	return (close(fd[io]), !io);
}

int main(void)
{
	int fd = ft_popen("ls", (char *[]) {"ls", NULL}, 'r');
	static char buf[4096];

	int size = read(fd, buf, 4096);

	printf("|BUF: %d %s|\n", size, buf);

	return 0;
}
