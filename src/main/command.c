#include "../../inc/minishell.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>



char	*find_command(t_minishell *shell)
{
	char	*path;
	int		i;
	char	*command_path;
	int		fd;

	path = ft_getenv(shell, "PATH");
	if (!path)
	{
		print_error(shell->command_array[0], "No such file or directory");
		free_all(shell);
		exit (127);
	}
	/* printf("command array before search: %s\n", shell->command_array[0]); */
	shell->path_array = split_while_skipping_quotes(shell, path, ':');
	free_and_reset_ptr((void **)&path);
	i = 0;
	while (shell->path_array[i])
	{
		command_path = ft_strjoin(shell, shell->path_array[i], "/");
		command_path = append_suffix(shell, command_path, shell->command_array[0]);
		/*
		command_path = ft_strjoin(shell, command_path, input_array[0]); */
		fd = access(command_path, F_OK & X_OK);
		if (fd == 0)
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
	int		is_builtin;
	char	*relative_path;
	int	custom_errno;

	path = NULL;
	/* printf("execute check envp: %s\n", shell->envp[0]); */
	custom_errno = -1;
	is_builtin = ft_is_builtin(shell, command_array, &exit_status, &custom_errno);
	/* printf("is builtin? %d\n", is_builtin); */
	/* printf("address of command array after builtin check: %p\n", (void *)command_array[0]); */
	if (!is_builtin)
	{
		relative_path = ft_strjoin(shell, "./", shell->command_array[0]);
		if (shell->command_array[0][0] == '/' || ft_strncmp(shell->command_array[0], "./", 2) == 0 || access(relative_path, F_OK) != -1)
		{

			path = ft_strdup(shell, shell->command_array[0]);
		}

		else
		{
			free_and_reset_ptr((void **)&relative_path);
			path = find_command(shell);
		}


		/* printf("command array after search: %s\n", shell->command_array[0]); */
		if (path)
		//free_child(shell);
			execve(path, shell->command_array, shell->envp);
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
			exit_status = 1;
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

	shell->command_array = split_while_skipping_quotes(shell, command, ' ');
	i = 0;
	while (shell->command_array[i])
	{
		shell->command_array[i] = remove_metaquotes(shell,
				shell->command_array[i]);
		/* printf("after remove: %s\n", shell->command_array[i]); */
		/* printf("address of command array: %p\n", (void *)shell->command_array[i]); */
		i++;
	}
	if (ft_strcmp(shell->command_array[0], ".") == 0)
	{
		print_error(".", "filename argument required");
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
