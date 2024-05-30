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
	/* printf("address of old token: %p\n", (void *)str); */
	free_and_reset_ptr((void **)&str);
	return (new_str);
}

char	*find_command(t_minishell *shell, char **input_array)
{
	char	*path;
	int		i;
	char	*command_path;
	int		fd;

	/* path = getenv("PATH");
	if (!path)
		return (error_free_all(shell, ERR_PATH_NOT_FOUND, NULL, NULL), NULL); */
	path = ft_getenv(shell, "PATH");
	if (!path)
	{
		print_error(input_array[0], "Permission denied");
		exit (126);
	}
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

void	execute_command_array(t_minishell *shell, char **command_array)
{
	char	*path;
	int		exit_status;
	char	*custom_message;
	exit_status = 0;
	int		is_builtin;

	(void)command_array;
	path = NULL;
	is_builtin = ft_is_builtin(shell, command_array, &exit_status);
	/* printf("is builtin? %d\n", is_builtin); */

	if (!is_builtin)
	{
		if (shell->command_array[0][0] == '/' || strncmp(shell->command_array[0], "./", 2) == 0 || access(ft_strjoin(shell, "./", shell->command_array[0]), F_OK) != -1)
			path = shell->command_array[0];
		else
			path = find_command(shell, shell->command_array);
		//free_child(shell);
		execve(path, shell->command_array, shell->envp);
		// TODO: also set exit status and custom message for builtins
		exit_status = 1;
	}
	if ((exit_status != 0 /* && ft_strcmp_btin(shell->command_array[0], "exit") != 0)
	|| (exit_status != 1 && ft_strcmp_btin(shell->command_array[0], "exit") == 0 */))
	{
		/* printf("child exit status %d not successful\n", exit_status); */
		custom_message = NULL;
		if (ft_strcmp_btin(shell->command_array[0], "cd") != 0)
			exit_status = set_exit_status_before_termination(shell,
				&custom_message);
		print_error(shell->command_array[0], custom_message);
	}
	/* printf("child exit status: %d\n", exit_status); */
	if (!shell->stay_in_parent)
	{
		/* printf("child pid: %d\n", getpid()); */
		if (is_builtin)
			free_iteration(shell);
		exit(exit_status);
	}

	else
	 	shell->last_exit_status = exit_status;

}

void	execute_command(t_minishell *shell, char *command)
{

	int		i;

	//bool	error;

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
	if (ft_strcmp(shell->command_array[0], ".") == 0)
	{
		print_error(".", "filename argument required");
		exit (2);
	}
	if (ft_strcmp(shell->command_array[0], "..") == 0)
	{
		print_error("..", "command not found");
		exit (127);
	}
	execute_command_array(shell, shell->command_array);

}
