#include "../../inc/minishell.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

char	*find_command(t_minishell *shell)
{
	int			i;
	struct stat	path_stat;

	shell->path = ft_getenv(shell, "PATH");
	if (!shell->path || !shell->path[0])
	{
		if (stat(shell->command_array[0], &path_stat) == 0)
		{
			print_error(shell->command_array[0], "Permission denied");
			free_all(shell);
			exit(126);
		}
		else
		{
			print_error(shell->command_array[0], "No such file or directory");
			free_all(shell);
			exit(127);
		}
	}
	split_while_skipping_quotes(shell, shell->path, ':');
	i = 0;
	while (shell->path_array[i])
	{
		shell->command_path = append(shell, shell->path_array[i], "/",
				FREE_NONE);
		shell->command_path = append(shell, shell->command_path,
				shell->command_array[0], FREE_BASE);
		if (access(shell->command_path, F_OK & X_OK) == 0)
			return (shell->command_path);
		free_and_reset_ptr((void **)&shell->command_path);
		i++;
	}
	return (NULL);
}
bool	parse_command(t_minishell *shell)
{
	if (ft_strcmp(shell->command_array[0], ".") == 0)
	{
		shell->custom_errno = U_NO_FILENAME_ARGUMENT;
		shell->my_exit_status = 2;
		return (false);
	}
	if (ft_strcmp(shell->command_array[0], "..") == 0)
	{
		errno = EFAULT;
		shell->my_exit_status = 127;
		return (false);
	}
	return (true);
}

char	*get_command_path(t_minishell *shell)
{
	struct stat	path_stat;
	char		cwd[PATH_MAX];

	if (!getcwd(cwd, PATH_MAX))
		error_free_all(shell, ERR_GETCWD, NULL, NULL);
	if (ft_strcmp(cwd, "/usr/bin") == 0)
	{
		shell->command_path = ft_strjoin(shell, "./", shell->command_array[0]);
	}
	else if (shell->command_array[0][0] == '/'
		|| ft_strncmp(shell->command_array[0], "./", 2) == 0)
	{
		if (stat(shell->command_array[0], &path_stat) == -1)
		{
			shell->custom_errno = U_NO_FILE;
			shell->my_exit_status = 127;
			return (NULL);
		}
		else if (S_ISDIR(path_stat.st_mode))
		{
			shell->custom_errno = U_IS_DIR;
			shell->my_exit_status = 126;
			return (NULL);
		}
		shell->command_path = ft_strdup(shell, shell->command_array[0]);
	}
	else
		shell->command_path = find_command(shell);
	return (shell->command_path);
}

int	execute_command_array(t_minishell *shell)
{
	char	*custom_message;
	bool	print_message;
	bool	builtin;

	print_message = true;
	builtin = is_builtin(shell, shell->command_array[0]);
	if (shell->builtin == B_ENV && ft_strcmp(shell->command_array[0],
			"env") != 0)
		print_message = false;
	if (shell->builtin != NOT_BUILTIN)
		shell->my_exit_status = handle_builtin(shell, &shell->custom_errno);
	else
	{
		shell->command_path = get_command_path(shell);
		if (shell->command_path)
		{
			execve(shell->command_path, shell->command_array, shell->envp);
			shell->my_exit_status = 1;
		}
	}
	if ((errno > 0 || shell->custom_errno > 0 || shell->my_exit_status != 0)
		&& print_message)
	{
		custom_message = NULL;
		set_exit_status_before_termination(shell, &custom_message,
			&shell->my_exit_status, shell->custom_errno);
		if (shell->my_exit_status == 127)
			print_error(shell->command_array[0], custom_message);
		else
			print_error(shell->command_array[1], custom_message);
	}
	return (shell->my_exit_status);
}
bool	prepare_command_array(t_minishell *shell, char *command)
{
	int	i;

	if (!command || !command[0])
		return (false);
	if (shell->pipes_total > 0)
		split_while_skipping_quotes(shell, command, ' ');
	i = 0;
	while (shell->command_array[i])
	{
		shell->command_array[i] = remove_metaquotes(shell,
				shell->command_array[i]);
		i++;
	}
	update_value(shell, "_", shell->command_array[i - 1], false);
	return (true);
}

void	execute_command(t_minishell *shell, char *command)
{
	int	my_exit_status;

	if (prepare_command_array(shell, command))
	{
		if (parse_command(shell))
			shell->my_exit_status = execute_command_array(shell);
	}
	if (!shell->stay_in_parent)
	{
		my_exit_status = shell->my_exit_status;
		free_all(shell);
		exit(my_exit_status);
	}
	else
		shell->last_exit_status = shell->my_exit_status;
}
