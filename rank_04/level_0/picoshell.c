/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 12:47:49 by marvin            #+#    #+#             */
/*   Updated: 2025/11/14 12:47:49 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>

int    picoshell(char **cmds[])
{
	int		pipefd[2];
	int		inpfd;
	int		i;
	pid_t	pid;

	if (!cmds || !cmds[0] || !cmds[0][0])
		return (1);
	i = 0;
	inpfd = -1;
	while (cmds[i])
	{
		if (cmds[i + 1])
		{
			if (pipe(pipefd) == -1)
				return (1);
		}
		else
		{
			pipefd[0] = -1;
			pipefd[1] = -1;
		}
		pid = fork();
		if (pid < 0)
		{
			if (pipefd[0] != -1)
				close(pipefd[0]);
			if (pipefd[1] != -1)
				close(pipefd[1]);
			if (inpfd != -1)
				close(inpfd);
			return (1);
		}
		else if (pid == 0)
		{
			if (inpfd != -1)
			{
				if (dup2(inpfd, STDIN_FILENO) == -1)
					exit(1);
				close(inpfd);
			}
			if (pipefd[1] != -1)
			{
				if (dup2(pipefd[1], STDOUT_FILENO) == -1)
					exit(1);
				close(pipefd[0]);
				close(pipefd[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		else
		{
			if (inpfd != -1)
				close(inpfd);
			if (pipefd[1] != -1)
				close(pipefd[1]);
			if (pipefd[0] != -1)
				inpfd = pipefd[0];
			else
				inpfd = -1;
		}
		i++;
	}
	if (inpfd != -1)
		close(inpfd);
	while (wait(NULL) > 0)
		;
	return (0);
}
int main(int argc, char **argv)
{
    int cmds_size = 1;
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "|"))
            cmds_size++;
    }
    char ***cmds = calloc(cmds_size + 1, sizeof(char **));
    if (!cmds)
    {
        dprintf(2, "Malloc error: %m\n");
        return 1;
    }
    cmds[0] = argv + 1;
    int cmds_i = 1;
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "|"))
        {
            cmds[cmds_i] = argv + i + 1;
            argv[i] = NULL;
            cmds_i++;
        }
    }
    int ret = picoshell(cmds);
    if (ret)
        perror("picoshell");
    free(cmds);
    return ret;
}
