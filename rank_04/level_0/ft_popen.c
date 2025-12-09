/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_popen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 11:35:03 by marvin            #+#    #+#             */
/*   Updated: 2025/11/21 11:35:03 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int     ft_popen(const char *file, char *const argv[], char type)
{
    int     pipefd[2];
    pid_t   pid;

    if (!file || !argv || (type != 'r' && type != 'w'))
        return (-1);
    if (pipe(pipefd) == -1)
        return (-1);
    pid = fork();
    if (pid == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return (-1);
    }
    else if (pid == 0)
    {
        if (type == 'r')
        {
            close(pipefd[0]);
            if (dup2(pipefd[1], STDOUT_FILENO) == -1)
                exit(1);
            close(pipefd[1]);
        }
        else
        {
            close(pipefd[1]);
            if (dup2(pipefd[0], STDIN_FILENO) == -1)
                exit(1);
            close(pipefd[0]);
        }
        execvp(file, argv);
        exit(1);
    }
    else
    {
        if (type == 'r')
        {
            close(pipefd[1]);
            return (pipefd[0]);
        }
        else
        {
            close(pipefd[0]);
            return (pipefd[1]);
        }
    }
    return (-1);
}

int     main(void)
{
    int     fd;
    char    buff[256];
    int     bytes;

    fd = ft_popen("ls", (char *const []){"ls", "-l", NULL}, 'r');
    if (fd == -1)
    {
        perror("ft_popen");
        return (-1);
    }
    while ((bytes = read(fd, buff, sizeof(buff) - 1)) > 0)
    {
        buff[bytes] = '\0';
        write(1, buff, bytes);
    }
    close(fd);
    wait(NULL);
    return (0);
}