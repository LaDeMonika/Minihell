#include "../../inc/minishell.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>



char	*find_command(t_minishell *shell)
{
	char	*path;
	int		i;
	char	*command_path;
	struct stat path_stat;


	path = ft_getenv(shell, "PATH");
	/* printf("path: %s\n", path); */
	if (!path || !path[0])
	{
		if(stat(shell->command_array[0], &path_stat) == 0)
		{
			print_error(shell->command_array[0], "Permission denied");
			free_all(shell);
			exit (126);
		}
		else
		{
			print_error(shell->command_array[0], "No such file or directory");
			free_all(shell);
			exit (127);
		}

	}
	shell->path_array = split_while_skipping_quotes(shell, path, ':');
	free_and_reset_ptr((void **)&path);
	i = 0;
	while (shell->path_array[i])
	{
		command_path = ft_strjoin(shell, shell->path_array[i], "/");
		command_path = append_suffix(shell, command_path, shell->command_array[0]);
		/* printf("command path: %s\n", command_path); */
		/*
		command_path = ft_strjoin(shell, command_path, input_array[0]); */
		if (access(command_path, F_OK & X_OK) == 0)
			return (command_path);
		free_and_reset_ptr((void **)&command_path);
		i++;
	}
	return (NULL);
}

void	execute_command_array(t_minishell *shell, char **command_array)
{
	char	*path;
	int		exit_status;
	char	*custom_message;
	exit_status = 0;
	int	custom_errno;
	struct stat path_stat;
	char	cwd[1024];

	path = NULL;
	/* printf("execute check envp: %s\n", shell->envp[0]); */
	custom_errno = -1;
	shell->builtin = check_builtin(command_array[0]);
	if (shell->builtin != NOT_BUILTIN)
		handle_builtin(shell, command_array, &exit_status, &custom_errno);
	else
	{
		if (getcwd(cwd, sizeof(cwd)) && ft_strcmp(cwd, "/usr/bin") == 0)
		{
			path = ft_strjoin(shell, "./", shell->command_array[0]);
		}

		/* if (shell->command_array[0][0] == '/' || ft_strncmp(shell->command_array[0], "./", 2) == 0 || access(relative_path, F_OK) != -1) */
		else if (shell->command_array[0][0] == '/' || ft_strncmp(shell->command_array[0], "./", 2) == 0)
		{
			//free_and_reset_ptr((void **)&relative_path);
			if(stat(shell->command_array[0], &path_stat) == -1)
			{
				print_error(shell->command_array[0], "No such file or directory");
				free_all(shell);
				exit (127);
			}
			if (S_ISDIR(path_stat.st_mode))
			{
				print_error(shell->command_array[0], "Is a directory");
				free_all(shell);
				exit (126);
			}
			path = ft_strdup(shell, shell->command_array[0]);
		}
		else
		{
			//free_and_reset_ptr((void **)&relative_path);
			path = find_command(shell);
		}


		//
		//free_child(shell);
		if (path)
			execve(path, shell->command_array, shell->envp);
		/* print_error(shell->command_array[0], "command not found");
		free_all(shell);
		exit (127); */
		// TODO: also set exit status and custom message for builtins
		free_and_reset_ptr((void **)&path);
		exit_status = 1;
	}
	if ((exit_status != 0 /* && ft_strcmp_btin(shell->command_array[0], "exit") != 0)
	|| (exit_status != 1 && ft_strcmp_btin(shell->command_array[0], "exit") == 0 */))
	{
		/* printf("child exit status %d not successful\n", exit_status); */
		custom_message = NULL;
		set_exit_status_before_termination(shell, &custom_message, &exit_status, custom_errno);
		if (ft_strcmp_btin(shell->command_array[0], "cd") == 0)
		{
			exit_status = 1;
			print_error(shell->command_array[1], custom_message);
		}
		else
		/* printf("errno in comamnd function: %d\n", errno); */
			print_error(shell->command_array[0], custom_message);
	}
	/* printf("child exit status: %d\n", exit_status); */
	if (!shell->stay_in_parent)
	{
		/* printf("child pid: %d\n", getpid()); */
		free_all(shell);
		exit(exit_status);
	}

	else
	 	shell->last_exit_status = exit_status;

}

void	execute_command(t_minishell *shell, char *command)
{
	int		i;

	if (!command || !command[0])
	{
		free_all(shell);
		exit(EXIT_SUCCESS);
	}
	if (shell->pipes_total > 0)
		shell->command_array = split_while_skipping_quotes(shell, command, ' ');
	i = 0;
	while (shell->command_array[i])
	{
		shell->command_array[i] = remove_metaquotes(shell,
				shell->command_array[i]);
		/* printf("after remove: [%s]\n", shell->command_array[i]); */
		/* printf("address of command array: %p\n", (void *)shell->command_array[i]); */
		i++;
	}

	update_value(shell, ft_strdup(shell, "_"), shell->command_array[i - 1], false);
	if (ft_strcmp(shell->command_array[0], ".") == 0)
	{
		print_error(".", "filename argument required");
		print_error(".", "usage: . filename [arguments]");
		free_all(shell);
		exit (2);
	}
	if (ft_strcmp(shell->command_array[0], "..") == 0)
	{
		print_error("..", "command not found");
		free_all(shell);
		exit (127);
	}
	execute_command_array(shell, shell->command_array);
}
