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

	infile = open(argv[1], O_RDONLY);//abre argv[1] con flag de solo lectura y lo guarda en infile
	if (infile == -1)//error si falla
		ft_error("infile error");
	close(fd[0]);//cierra lectura (no necesario)
	dup2(infile, STDIN_FILENO);//redirecciona la entrada a infile
	dup2(fd[1], STDOUT_FILENO);//redirecciona la salida a la escritura
	ft_execve(argv[2], envp);//ejecuta (comando_1)
}

static void	ft_child_process_two(char **argv, char **envp, int *fd)
{
	int	outfile;

	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);//escribe en argv[4] con flags: solo escritura, si no existe lo crea, si existe lo limpia, define permisos
	if (outfile == -1)//error si falla
		ft_error("outfile error");
	close(fd[1]);//cierra escritura (no necesario)
	dup2(fd[0], STDIN_FILENO);//redirecciona la entrada a la lectura
	dup2(outfile, STDOUT_FILENO);//redirecciona la salida a outfile
	ft_execve(argv[3], envp);//ejecuta (comando_2)
}

static void	ft_close_and_wait(int fd[2], pid_t pid_1, pid_t pid_2)
{
	close(fd[0]);//cerrar lectura
	close(fd[1]);//cerrar escritura
	waitpid(pid_1, NULL, 0);
	waitpid(pid_2, NULL, 0);
}

int	main(int argc, char **argv, char **envp)
{
	int		fd[2];//fd del pipe [0] -> lectura ; [1] -> escritura
	pid_t	pid_1;//proceso hijo 1
	pid_t	pid_2;//proceso hijo 2

	if (argc == 5)
	{
		if (pipe(fd) == -1)//crear el pipe y error si falla
			return (ft_error("pipe error"), 1);
		pid_1 = fork();//crear proceso hijo 1
		if (pid_1 == -1)//error si falla
			return (ft_error("fork 1 error"), 1);
		else if (pid_1 == 0)//si es el proceso del hijo 1
			ft_child_process_one(argv, envp, fd);
		pid_2 = fork();//crear proceso hijo 2
		if (pid_2 == -1)//error si falla
			return (ft_error("fork 2 error"), 1);
		else if (pid_2 == 0)//si es el proceso del hijo 2
			ft_child_process_two(argv, envp, fd);
		return (ft_close_and_wait(fd, pid_1, pid_2), 0);//proceso padre cierra los fd y espera a que los dos procesos hijos terminen
	}
	else//mensaje de error si no tiene 5 args
	{
		ft_putstr_fd("\033[1;31mPlease enter './pipex infile comand_1", 2);
		return (ft_putstr_fd(" comand_2 outfile'\033[0m\n", 2), 1);
	}
}
