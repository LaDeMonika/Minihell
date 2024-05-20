#include "../../inc/minishell.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

int	count_literal_chars(char *str, char metaquote)
{
	int	i;
	int	len;

	i = 0;
	len = 0;

	while (str && str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			if (!(metaquote))
				metaquote = str[i];
			else if (metaquote == str[i])
				metaquote = '\0';
			else if (str[i] != metaquote)
				len++;
		}
		else
			len++;
		i++;
	}
	return (len);
}

/*first determine new string length and then create new string without outer quotes*/
char	*remove_metaquotes(t_minishell *shell, char *str)
{
	char	*new_str;
	int		new_len;
	int		i;
	int		j;
	char	metaquote;

	metaquote = '\0';
	new_len = count_literal_chars(str, metaquote);
	new_str = try_malloc(shell, sizeof(char) * (new_len + 1));
	i = 0;
	j = 0;
	while (str && str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			if (!(metaquote))
				metaquote = str[i];
			else if (metaquote == str[i])
				metaquote = '\0';
			else if (str[i] != metaquote)
			{
				new_str[j] = str[i];
				j++;
			}

		}
		else
		{
			new_str[j] = str[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0';
	free(str);
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
	shell->path_array = split_while_skipping_quotes(shell, path, ':');
	i = 0;
	while (shell->path_array[i])
	{
		command_path = ft_strjoin(shell, shell->path_array[i], "/");
		command_path = ft_strjoin(shell, command_path, input_array[0]);
		fd = access(command_path, F_OK & X_OK);
		if (fd == 0)
			return (command_path);
		i++;
	}
	return (NULL);
}

void	execute_command(t_minishell *shell, char *command)
{
	char	*path;
	int		exit_status;
	char	*custom_message;
	int		i;
	int		is_builtin;
	bool	error;

	/* printf("pid: %d\n", getpid());
	printf("command: %s\n", command); */
	if (!command || !command[0])
		exit(EXIT_SUCCESS);
	shell->command_array = split_while_skipping_quotes(shell, command, ' ');
	i = 0;
	/* if (!shell->command_array[0])
		exit(EXIT_SUCCESS); */
	while (shell->command_array[i])
	{
		/* printf("before remove: %s\n", shell->command_array[i]); */
		shell->command_array[i] = remove_metaquotes(shell,
				shell->command_array[i]);
		/* printf("after remove: %s\n", shell->command_array[i]); */
		i++;
	}

	// builtins
	is_builtin = ft_is_builtin(shell, shell->command_array);
	path = NULL;
	error = false;
	if (is_builtin == 2)
	{
		if (shell->command_array[0][0] == '/' || strncmp(shell->command_array[0], "./", 2) == 0)
			path = shell->command_array[0];
		else
			path = find_command(shell, shell->command_array);
		execve(path, shell->command_array, shell->envp);
		// TODO: also set exit status and custom message for builtins
		error = true;
	}
	if (is_builtin == 1)
		error = true;
	if (error)
	{
		custom_message = NULL;
		exit_status = set_exit_status_before_termination(shell,
				&custom_message);
		print_error(shell->command_array[0], custom_message);
		if (is_builtin == 1)
			exit_status = 1;
	}
	else
		exit_status = 0;
	/* printf("child exit status: %d\n", exit_status); */
	if (!shell->is_cd_in_parent)
		exit(exit_status);
	else
	 	shell->last_exit_status = exit_status;
}
