/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:21:02 by msimic            #+#    #+#             */
/*   Updated: 2024/06/11 12:38:53 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*get_new_pwd(t_minishell *shell)
{
	char	*new_pwd;

	if (!shell->command_array[1] || ft_strcmp(shell->command_array[1], "--") == 0
		|| ft_strcmp(shell->command_array[1], "~") == 0)
		new_pwd = ft_getenv(shell, "HOME");
	else if (shell->command_array[1][0] == '~')
		new_pwd = ft_strjoin(shell, getenv("HOME"), shell->command_array[1] + 1);
	else if (ft_strcmp(shell->command_array[1], "-") == 0)
	{
		new_pwd = ft_getenv(shell, "OLDPWD");
		printf("%s\n", new_pwd);
	}
	else if (ft_strcmp(shell->command_array[1], "..") == 0)
		new_pwd = ft_strdup(shell, "..");
	else
		new_pwd = ft_strdup(shell, shell->command_array[1]);
	return (new_pwd);
}

int	ft_cd(t_minishell *shell, int *custom_errno)
{
	if (shell->command_array[1] && shell->command_array[2])
	{
		*custom_errno = U_TOO_MANY_ARGUMENTS;
		return (1);
	}
    shell->current_pwd = getcwd(NULL, 0);
	if (shell->current_pwd == NULL)
		error_free_all(shell, ERR_GETCWD, NULL, NULL);
	shell->new_pwd = get_new_pwd(shell);
	if (chdir(shell->new_pwd) == -1)
	{
		if (access(shell->new_pwd, F_OK) == -1)
			*custom_errno = U_NO_FILE;
		else if (access(shell->new_pwd, R_OK) == -1)
			*custom_errno = U_NO_PERMISSION;
		return (1);
	}
	update_value(shell, "OLDPWD", shell->current_pwd, false);
	free_and_reset_ptr((void **)&shell->current_pwd);
	shell->current_pwd = getcwd(NULL, 0);
	update_value(shell, "PWD", shell->current_pwd, false);
	return (0);
}
