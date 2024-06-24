/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:37:31 by lilin             #+#    #+#             */
/*   Updated: 2024/06/24 18:37:32 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	handle_builtin_for_each_arg(t_minishell *shell, int *custom_errno)
{
	int	i;
	int	exit_status;

	exit_status = 0;
	i = 1;
	while (shell->command_array[i])
	{
		if (shell->builtin == B_EXPORT)
		{
			exit_status = ft_export(shell, shell->command_array[i],
					custom_errno);
			free_and_reset_ptr((void **)&shell->new_key);
			free_and_reset_ptr((void **)&shell->new_value);
		}
		else if (shell->builtin == B_UNSET)
		{
			exit_status = ft_unset(shell, shell->command_array[i],
					custom_errno);
		}
		i++;
	}
	return (exit_status);
}

int	handle_builtin(t_minishell *shell, int *custom_errno)
{
	if (shell->builtin == B_CD)
		return (ft_cd(shell, custom_errno));
	else if (shell->builtin == B_ECHO)
		return (ft_echo(shell));
	else if (shell->builtin == B_ENV)
		return (ft_env(shell, custom_errno));
	else if (shell->builtin == B_EXIT)
		return (ft_exit(shell, custom_errno));
	else if (shell->builtin == B_EXPORT)
	{
		if (!shell->command_array[1])
			return (export_no_args(shell));
		return (handle_builtin_for_each_arg(shell, custom_errno));
	}
	else if (shell->builtin == B_PWD)
		return (ft_pwd(shell));
	else if (shell->builtin == B_UNSET)
		return (handle_builtin_for_each_arg(shell, custom_errno));
	return (0);
}

bool	is_builtin(t_minishell *shell, char *token)
{
	if (ft_strncmp(token, "cd", 3) == 0)
		shell->builtin = B_CD;
	else if (ft_strcmp(token, "echo") == 0)
		shell->builtin = B_ECHO;
	else if (ft_strcmp(token, "env") == 0)
		shell->builtin = B_ENV;
	else if (ft_strcmp(token, "exit") == 0)
		shell->builtin = B_EXIT;
	else if (ft_strcmp(token, "export") == 0)
		shell->builtin = B_EXPORT;
	else if (ft_strcmp(token, "pwd") == 0)
		shell->builtin = B_PWD;
	else if (ft_strcmp(token, "unset") == 0)
		shell->builtin = B_UNSET;
	else
	{
		shell->builtin = NOT_BUILTIN;
		return (false);
	}
	return (true);
}
