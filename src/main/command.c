#include "../../inc/minishell.h"
#include <stdlib.h>
#include <unistd.h>

char	*remove_quotes(char *command)
{
	char *new_str;
	int	new_len;
	int	i;
	int	j;

	i = 0;
	new_len = 0;
	while (command[i])
	{
		if (command[i] != '"' && command[i] != '\'')
			new_len++;
		i++;
	}
	new_str = malloc(sizeof(char) * (new_len + 1));
	i = 0;
	j = 0;
	while (command[i])
	{
		if (command[i] != '"' && command[i] != '\'')
		{
			new_str[j] = command[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

char	*find_command(char **input_array)
{
	char	*path;
	char	**path_array;
	int		i;
	char	*command_path;
	int		fd;

	path = getenv("PATH");
	if (!path)
		return (ft_error_msg(ERR_PATH_NOT_FOUND), NULL);
	path_array = ft_split(path, ':');
	i = 0;
	while (path_array[i])
	{
		command_path = ft_strjoin(path_array[i], "/");
		command_path = ft_strjoin(command_path, input_array[0]);
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
	*exit_status = EXIT_FAILURE;
	if (errno == EACCES)
		*exit_status = 126;
	else if (errno == EFAULT || errno == ENOENT)
	{
		*exit_status = 127;
		if (errno == EFAULT && !shell->error)
			return ("command not found");
	}
	return (NULL);
}


void	execute_command(t_minishell *shell, char *command)
{
	int		is_builtin;
	char	**command_array;
	char	*path;
	int		exit_status;
	char	*custom_message;
	int	i;

	(void)shell;
	command_array = ft_split(command, ' ');
	i = 0;
	while (command_array[i])
	{
		command_array[i] = remove_quotes(command_array[i]);
		i++;
	}
	
	// builtins
	is_builtin = ft_is_builtin(shell, command_array);
	path = NULL;
	if (strncmp(command_array[0], "./", 2) != 0)
		path = find_command(command_array);
	else
		path = command_array[0];
	//dup2(shell->stderr_copy, STDERR_FILENO);
	if (shell->error)
		exit(EXIT_FAILURE);
	if (is_builtin == 1)
	{
		redirect_errors();
		execve(path, command_array, shell->envp);
	}
	// TODO: also set exit status and custom message for builtins
	//write(2, "execve skipped\n", 16);

	/* if (shell->error)
	{
		write(2, "error before execve\n", 21);
		write(2, shell->error, ft_strlen(shell->error));
		write(2, "\n", 1);
	} */

	/* dup2(shell->stderr_copy, STDERR_FILENO);
	close(shell->stderr_copy); */
	custom_message = set_exit_status(shell, &exit_status);
	if (custom_message)
		append_custom_error_to_log(command_array[0], custom_message);
		/* custom_perror(ft_strjoin(command_array[0], ": "), custom_message); */
	else
	{
		append_strerror_to_log(command_array[0], errno);
		/* perror(ft_strjoin(command_array[0], ": ")); */
	}
	/* printf("command: %s errno: %d\n", command_array[0], errno);
	printf("command: %s exit status: %d\n", command_array[0], exit_status); */
	exit(exit_status);
}

