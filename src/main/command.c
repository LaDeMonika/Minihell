#include "../../inc/minishell.h"

char	*find_command(t_minishell *shell, char **input_array)
{
	char	*path;
	int		i;
	char	*command_path;
	int		fd;

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

	if (!is_builtin)
	{
		if (shell->command_array[0][0] == '/' || strncmp(shell->command_array[0], "./", 2) == 0 || access(ft_strjoin(shell, "./", shell->command_array[0]), F_OK) != -1)
			path = shell->command_array[0];
		else
			path = find_command(shell, shell->command_array);
		execve(path, shell->command_array, shell->envp);
		exit_status = 1;
	}
	if ((exit_status != 0))
	{
		custom_message = NULL;

		exit_status = set_exit_status_before_termination(shell,	&custom_message);
		if (ft_strcmp_btin(shell->command_array[0], "cd") == 0)
			exit_status = 1;
		print_error(shell->command_array[0], custom_message);
	}
	if (!shell->stay_in_parent)
	{
		if (is_builtin)
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
		exit(EXIT_SUCCESS);
	shell->command_array = split_while_skipping_quotes(shell, command, ' ');
	i = 0;
	while (shell->command_array[i])
	{
		shell->command_array[i] = remove_metaquotes(shell,
				shell->command_array[i]);
		i++;
	}
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
