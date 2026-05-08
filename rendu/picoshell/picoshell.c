#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define IN 0
#define OUT 1
#define PREV 2

void child(int *fd, bool last, char **cmd) {
    if (fd[PREV] != -1) {
        dup2(fd[PREV], IN);
        close(fd[PREV]);
    }
    if (!last) {
		dup2(fd[OUT], OUT);
    	close(fd[IN]);
    	close(fd[OUT]);
    }
	execvp(cmd[0], cmd);
	exit(1);
}

void parent(int *fd, bool last) {
    // If prev is open, close it!
	if (fd[PREV] != -1)
        close(fd[PREV]);

	// If it's not the last close the write end
    // and put the read end as PREV for the next cmd!
    if (!last) {
        close(fd[OUT]);
    	fd[PREV] = fd[IN];
    }
    // If it's the last put the PREV as -1, it's not gona be used anymore,
    // Since the PREV is the same as the IN which needs to be open
    else {
        fd[PREV] = -1;
    }
}

int picoshell(char **cmds[]) {
	int pid;
	int fd[3];

	// Set's the previous fd as -1, to know is not in use.
	fd[PREV] = -1;
	if (!cmds || !*cmds || !**cmds)
		return 1;

	int i = 0;
	while (cmds[i]) {
		// pipes only if it's not the last cmd
		if (cmds[i + 1] && pipe(fd) == -1)
			return 1;
		pid = fork();
		if (pid == -1) {
			close(fd[IN]);
			close(fd[OUT]);
			if (fd[PREV] != -1)
				close(fd[PREV]);
			return 1;
		}
		if (pid == 0)
			child(fd, cmds[i + 1] == NULL, cmds[i]);
		parent(fd, cmds[i + 1] == NULL);
		i++;
	}
	while (i-- > 0)
		wait(0);
	return 0;
}

int main(int argc, char **argv)
{
	char **cmds[argc];
	int i;
	int j;

	if (argc < 2)
		return (1);
	i = 1;
	j = 0;
	cmds[j++] = &argv[i];
	while (argv[i])
	{
		if (argv[i][0] == '|' && argv[i][1] == '\0')
		{
			argv[i] = 0;
			if (argv[i + 1])
				cmds[j++] = &argv[i + 1];
		}
		i++;
	}
	cmds[j] = 0;
	return (picoshell(cmds));
}
