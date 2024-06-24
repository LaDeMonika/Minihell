/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:31:37 by lilin             #+#    #+#             */
/*   Updated: 2024/06/24 18:34:01 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

char	*find_command_in_path_env(t_minishell *shell)
{
	struct stat	path_stat;
	int			i;

	shell->path = ft_getenv(shell, "PATH");
	if (!shell->path || !shell->path[0])
	{
		if (stat(shell->command_array[0], &path_stat) == 0)
			return (set_custom_errno(shell, U_NO_PERMISSION, 126), NULL);
		else
			return (set_custom_errno(shell, U_NO_FILE, 127), NULL);
	}
	shell->path_array = split_while_skipping_quotes(shell, shell->path, ':');
	shell->split_arr = NULL;
	i = 0;
	while (shell->path_array[i])
	{
		shell->command_path = ft_strjoin(shell, shell->path_array[i], "/");
		shell->command_path = append(shell, shell->command_path,
				shell->command_array[0], BASE);
		if (access(shell->command_path, F_OK & X_OK) == 0)
			return (shell->command_path);
		free_and_reset_ptr((void **)&shell->command_path);
		i++;
	}
	return (NULL);
}

bool	parse_command(t_minishell *shell, char *command)
{
	int	i;

	if (shell->pipes_total > 0)
	{
		shell->command_array = split_while_skipping_quotes(shell, command, ' ');
		shell->split_arr = NULL;
	}
	i = 0;
	while (shell->command_array[i])
	{
		shell->command_array[i] = remove_metaquotes(shell,
				shell->command_array[i]);
		i++;
	}
	update_value(shell, "_", shell->command_array[i - 1], false);
	if (ft_strcmp(shell->command_array[0], ".") == 0)
		return (set_custom_errno(shell, U_NO_FILENAME_ARGUMENT, 2), false);
	if (ft_strcmp(shell->command_array[0], "..") == 0)
		return (set_custom_errno(shell, EFAULT, 127), false);
	return (true);
}

char	*get_command_path(t_minishell *shell)
{
	struct stat	path_stat;

	try_getcwd(shell);
	if (ft_strcmp(shell->cwd, "/usr/bin") == 0)
		shell->command_path = ft_strjoin(shell, "./", shell->command_array[0]);
	else if (shell->command_array[0][0] == '/'
		|| ft_strncmp(shell->command_array[0], "./", 2) == 0)
	{
		if (stat(shell->command_array[0], &path_stat) == -1)
			return (set_custom_errno(shell, U_NO_FILE, 127), NULL);
		else if (S_ISDIR(path_stat.st_mode))
			return (set_custom_errno(shell, U_IS_DIRECTORY, 126), NULL);
		shell->command_path = ft_strdup(shell, shell->command_array[0]);
	}
	else
		shell->command_path = find_command_in_path_env(shell);
	return (shell->command_path);
}

int	execute_command_array(t_minishell *shell, bool print_message)
{
	if (is_builtin(shell, shell->command_array[0]))
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
	if ((errno > 0 || shell->custom_errno > -1 || shell->my_exit_status != 0)
		&& print_message)
		set_exit_status_before_termination(shell, &shell->my_exit_status,
			shell->custom_errno);
	return (shell->my_exit_status);
}

void	execute_command(t_minishell *shell, char *command)
{
	int	my_exit_status;

	if (command && command[0] && parse_command(shell, command))
		shell->my_exit_status = execute_command_array(shell, true);
	if (!shell->stay_in_parent)
	{
		my_exit_status = shell->my_exit_status;
		free_all(shell);
		exit(my_exit_status);
	}
	else
		shell->last_exit_status = shell->my_exit_status;
}
