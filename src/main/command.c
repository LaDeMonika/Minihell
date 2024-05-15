#include "../../inc/minishell.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

/*first determine new string length and then create new string without outer quotes*/
char	*remove_outer_quotes(char *command)
{
	char	*new_strtr;
	int		new_len;
	int		i;
	int		j;
	char	quote_type;

	i = 0;
	new_len = 0;
	quote_type = '\0';
	while (command[i])
	{
		if (command[i] == '"' || command[i] == '\'')
		{
			if (!quote_type)
				quote_type = command[i];
			else if (command[i] != quote_type)
				new_len++;
		}
		else
			new_len++;
		i++;
	}
	new_strtr = malloc(sizeof(char) * (new_len + 1));
	i = 0;
	j = 0;
	while (command[i])
	{
		if (command[i] != quote_type)
		{
			new_strtr[j] = command[i];
			j++;
		}
		i++;
	}
	new_strtr[j] = '\0';
	return (new_strtr);
}

char	*find_command(t_minishell *shell, char **input_array)
{
	char	*path;
	char	**path_array;
	int		i;
	char	*command_path;
	int		fd;

	path = getenv("PATH");
	if (!path)
		return (error_free_all(shell, ERR_PATH_NOT_FOUND), NULL);
	path_array = ft_split(path, ':');
	i = 0;
	while (path_array[i])
	{
		command_path = ft_strjoin(shell, path_array[i], "/");
		command_path = ft_strjoin(shell, command_path, input_array[0]);
		fd = access(command_path, F_OK & X_OK);
		if (fd == 0)
		{
			return (command_path);
		}
		i++;
	}
	return (NULL);
}
char	*set_exit_status(t_minishell *shell, int *exit_status)
{
	(void)shell;
	*exit_status = EXIT_FAILURE;
	if (errno == EACCES)
		*exit_status = 126;
	else if (errno == EFAULT || errno == ENOENT)
	{
		*exit_status = 127;
		if (errno == EFAULT)
			return ("command not found");
	}
	return (NULL);
}

void	execute_command(t_minishell *shell, char *command)
{
	char	**command_array;
	char	*path;
	int		exit_status;
	char	*custom_message;
	int		i;

	// int		is_builtin;
	(void)shell;
	command_array = split_skip_quotes(shell, command, ' ');
	i = 0;
	while (command_array[i])
	{
		command_array[i] = remove_outer_quotes(command_array[i]);
		i++;
	}
	// builtins
	// is_builtin = ft_is_builtin(shell, command_array);
	path = NULL;
	if (strncmp(command_array[0], "./", 2) != 0)
		path = find_command(shell, command_array);
	else
		path = command_array[0];
	execve(path, command_array, shell->envp);
	// TODO: also set exit status and custom message for builtins
	custom_message = set_exit_status(shell, &exit_status);
	if (custom_message)
		print_error(command_array[0], custom_message);
	else
		print_error(command_array[0], NULL);
	exit(exit_status);
}
