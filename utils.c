/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ptrapero <ptrapero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:30:16 by ptrapero          #+#    #+#             */
/*   Updated: 2025/03/21 20:30:16 by ptrapero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_error(char *message)
{
	ft_putstr_fd("\033[1;41;5m ERROR \033[0m\n", 2);
	ft_putstr_fd("\033[91m", 2);
	perror(message);
	ft_putstr_fd("\033[0m", 2);
	exit(EXIT_FAILURE);
}

static void	ft_free(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
}

char	*ft_get_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin3(paths[i], "/", cmd);
		if (access(path, X_OK) == 0)
		{
			ft_free(paths);
			return (path);
		}
		else
			free(path);
		i++;
	}
	ft_free(paths);
	return (NULL);
}

void	ft_execve(char *argv, char **envp)
{
	char	**cmd;
	char	*path;
	int		i;

	i = 0;
	cmd = ft_split(argv, ' ');
	if (!cmd || !cmd[0])
		ft_error("missing command");
	path = ft_get_path(cmd[0], envp);
	if (!path)
	{
		while (cmd[i])
			free(cmd[i++]);
		free(cmd);
		ft_error("path not found");
	}
	if (execve(path, cmd, envp) == -1)
		ft_error("execve error");
}
