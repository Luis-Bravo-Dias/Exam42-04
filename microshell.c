#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/types.h>

// int	ft_putstr_fd2(char *str, char *arg)
// {
// 	while (*str)
// 		write(2, str++. 1);
// 	if (arg)
// 		while (*arg)
// 			write(2, arg++, 1);
// 	write(2, "\n", 1);
// 	return (1);
// }

// int	main (int ac, char *av, char **env)
// {
// 	int	i;
// 	int fd[2];
// 	int fd_tmp;
// 	(void)ac;

// 	i = 0;
// 	fd_tmp = dup(STDIN_FILENO);
// 	while (av[i] && av[i + 1])
// 	{
// 		av = &av[i + 1];
// 		i = 0;

// 		while (av[i] && strcmp(av[i], ";") && strcmp(av[i], "|"))
// 			i++;
// 		if (strcmp(av[0], "cd") == 0)
// 		{
// 			if (i != 2)
// 				ft_putstr_fd2("error: cd: bad arguments", NULL);
// 			else if (chdir(av[1]) != 0)
// 				ft_putstr_fd2("error: cd: cannot change directory to", av[1]);
// 		}
// 		else if (i != 0 )
// 	}
	
// }

int	error_men(char *str)
{
	while (*str)
		write(2, str++, 1);
	return (1);
}

int	cd(char **av, int i)
{
	if (i != 2)
		return error_men("error: cd: bad arguments\n");
	else if (chdir(av[1]) == -1)
		return error_men("error: cd: cannot change directory to "), error_men(av[1]), error_men("\n");
	return (0);
}

int executer(char **av, char **env, int i)
{
	int	fd[2];
	int status;
	int pipe_in;
	int	pid;

	pipe_in = av[i] && !strcmp(av[i], "|");
	if (pipe_in && pipe(fd) == -1)
		return error_men("error: fatal\n");
	
	pid = fork();
	if (!pid)
	{
		av[i] = 0;
		if (pipe_in && (dup2(fd[1], 1) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
			return error_men("error: fatal\n");
		execve(*av, av, env);
		return error_men("error: cannot execute "), error_men(*av), error_men("\n");
	}
	waitpid(pid, &status, 0);
	if (pipe_in && (dup2(fd[0], 0) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
			return error_men("error: fatal\n");
	return WIFEXITED(status) && WEXITSTATUS(status);
}

int main(int ac, char **av, char **env)
{
	int i;
	int status;

	i = 0;
	status = 0;

	if (ac > 1)
	{
		while (av[i] && av[++i])
		{
			av += i;
			i = 0;
			while (av[i] && strcmp(av[i], "|") && strcmp(av[i], ";"))
				i++;
			if (!strcmp(*av, "cd"))
				status = cd(av, i);
			else if (i)
				status = executer(av, env, i);
		}
	}
	return (status);
}