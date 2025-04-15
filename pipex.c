/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptrapero <ptrapero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 20:40:48 by ptrapero          #+#    #+#             */
/*   Updated: 2025/03/06 20:40:48 by ptrapero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	ft_child_process_one(char **argv, char **envp, int *fd)
{
	int	infile;

	infile = open(argv[1], O_RDONLY);
	if (infile == -1)
		ft_error("infile error");
	close(fd[0]);
	dup2(infile, STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	ft_execve(argv[2], envp);
}

static void	ft_child_process_two(char **argv, char **envp, int *fd)
{
	int	outfile;

	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (outfile == -1)
		ft_error("outfile error");
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	dup2(outfile, STDOUT_FILENO);
	ft_execve(argv[3], envp);
}

static void	ft_close_and_wait(int fd[2], pid_t pid_1, pid_t pid_2)
{
	close(fd[0]);
	close(fd[1]);
	waitpid(pid_1, NULL, 0);
	waitpid(pid_2, NULL, 0);
}

int	main(int argc, char **argv, char **envp)
{
	int		fd[2];
	pid_t	pid_1;
	pid_t	pid_2;

	if (argc == 5)
	{
		if (pipe(fd) == -1)
			return (ft_error("pipe error"), 1);
		pid_1 = fork();
		if (pid_1 == -1)
			return (ft_error("fork 1 error"), 1);
		else if (pid_1 == 0)
			ft_child_process_one(argv, envp, fd);
		pid_2 = fork();
		if (pid_2 == -1)
			return (ft_error("fork 2 error"), 1);
		else if (pid_2 == 0)
			ft_child_process_two(argv, envp, fd);
		return (ft_close_and_wait(fd, pid_1, pid_2), 0);
	}
	else
	{
		ft_putstr_fd("\033[1;31mPlease enter './pipex infile comand_1", 2);
		return (ft_putstr_fd(" comand_2 outfile'\033[0m\n", 2), 1);
	}
}
