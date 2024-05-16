#include "../../inc/minishell.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

int	count_literal_chars(char *str, char *metaquote)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	*metaquote = '\0';
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			if (!(*metaquote))
				*metaquote = str[i];
			else if (str[i] != *metaquote)
				len++;
		}
		else
			len++;
		i++;
	}
	return (len);
}

/*first determine new string length and then create new string without outer quotes*/
char	*remove_metaquotes(t_minishell *shell, char *command)
{
	char	*new_str;
	int		new_len;
	int		i;
	int		j;
	char	metaquote;

	new_len = count_literal_chars(command, &metaquote);
	new_str = malloc(sizeof(char) * (new_len + 1));
	if (!new_str)
		return (error_free_all(shell, ERR_MALLOC, NULL, NULL), NULL);
	i = 0;
	j = 0;
	while (command[i])
	{
		if (command[i] != metaquote)
		{
			new_str[j] = command[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0';
	free(command);
	return (new_str);
}

char	*find_command(t_minishell *shell, char **input_array)
{
	char	*path;

	int		i;
	char	*command_path;
	int		fd;

	path = getenv("PATH");
	if (!path)
		return (error_free_all(shell, ERR_PATH_NOT_FOUND, NULL, NULL), NULL);
	shell->path_array = split_skip_quotes(shell,path, ':');
	i = 0;
	while (shell->path_array[i])
	{
		command_path = ft_strjoin(shell, shell->path_array[i], "/");
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
	char	*path;
	int		exit_status;
	char	*custom_message;
	int		i;

	// int		is_builtin;
	(void)shell;
	shell->command_array = split_skip_quotes(shell, command, ' ');
	i = 0;
	while (shell->command_array[i])
	{
		shell->command_array[i] = remove_metaquotes(shell, shell->command_array[i]);
		i++;
	}
	// builtins
	// is_builtin = ft_is_builtin(shell, command_array);
	path = NULL;
	if (strncmp(shell->command_array[0], "./", 2) != 0)
		path = find_command(shell, shell->command_array);
	else
		path = shell->command_array[0];
	execve(path, shell->command_array, shell->envp);
	// TODO: also set exit status and custom message for builtins
	custom_message = set_exit_status(shell, &exit_status);
	if (custom_message)
		print_error(shell->command_array[0], custom_message);
	else
		print_error(shell->command_array[0], NULL);
	exit(exit_status);
}
