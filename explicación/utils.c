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

void	ft_error(char *message)//mensaje y salida de error
{
	ft_putstr_fd("\033[1;41;5m ERROR \033[0m\n", 2);
	ft_putstr_fd("\033[91m", 2);
	perror(message);
	ft_putstr_fd("\033[0m", 2);
	exit(EXIT_FAILURE);
}

static void	ft_free(char **paths)//libera char doble puntero
{
	int	i;

	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
}

char	*ft_get_path(char *cmd, char **envp)//busca la ruta del ejecutable de un comando en las variables de entorno
{
	char	**paths;
	char	*path;
	int		i;//i marca en envp donde empieza la ruta -> ej: envp[3] == PATH=/usr/bin...

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)//busca en el entorno donde está la ruta
		i++;
	if (!envp[i])//control de errores si se acaba entorno
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');//(a partir del '=')separa por ':' y se guarda en **paths
	i = 0;
	while (paths[i])//busca si la ruta es válida, si no sigue buscando
	{
		path = ft_strjoin3(paths[i], "/", cmd);//concatena la ruta con el comando
		if (access(path, X_OK) == 0)//access comprueba si tiene permiso para acceder a un archivo o directorio, con su bandera X_OK se refiere a permiso de ejecución
		{
			ft_free(paths);//libera paths(split)
			return (path);//devuelve la ruta válida
		}
		else
			free(path);//libera(strjoin) y sigue buscando
		i++;
	}
	ft_free(paths);//error si no encuentra y libera
	return (NULL);
}

void	ft_execve(char *argv, char **envp)//ejecuta el comando pasando por un control de errores
{
	char	**cmd;
	char	*path;
	int		i;

	i = 0;
	cmd = ft_split(argv, ' ');//guarda cada parte del comando en el array de strings cmd
	if (!cmd || !cmd[0])//error si el comando esta vacío
		ft_error("missing command");
	path = ft_get_path(cmd[0], envp);//llama a get_path con el primer comando para sacar la ruta y la guarda en path
	if (!path)//libera(split) y sale si hay error en la ruta
	{
		while (cmd[i])
			free(cmd[i++]);
		free(cmd);
		ft_error("path not found");
	}
	if (execve(path, cmd, envp) == -1)//llama a execve y a su vez hace control de errores ; execve(ruta, comando, entorno);
		ft_error("execve error");
}
