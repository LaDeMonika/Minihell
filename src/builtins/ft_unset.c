/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:23:13 by msimic            #+#    #+#             */
/*   Updated: 2024/05/31 16:55:54 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	find_key(t_minishell *shell, char *key, int *i)
{
	*i = 0;
	while (shell->envp[*i])
	{
		shell->old_key = ft_substr(shell, shell->envp[*i], 0,
				index_of_char(shell->envp[*i], '='));
		if (ft_strcmp(shell->old_key, key) == 0)
		{
			free_and_reset_ptr((void **)&shell->old_key);
			return (true);
		}
		free_and_reset_ptr((void **)&shell->old_key);
		(*i)++;
	}
	return (false);
}

int	remove_pair(t_minishell *shell, int index_of_key)
{
	int	i;
	int	j;
	int	old_size;

	old_size = sizeof_array((void **)shell->envp);
	shell->new_envp = try_malloc(shell, sizeof(char *) * (old_size));
	i = 0;
	j = 0;
	while (shell->envp[i])
	{
		if (i != index_of_key)
		{
			shell->new_envp[j] = shell->envp[i];
			j++;
		}
		else
			free_and_reset_ptr((void **)&shell->envp[i]);
		i++;
	}
	shell->new_envp[j] = NULL;
	free_and_reset_ptr((void **)&shell->envp);
	shell->envp = shell->new_envp;
	shell->new_envp = NULL;
	return (0);
}

int	ft_unset(t_minishell *shell, char *key, int *custom_errno)
{
	int	index_of_key;

	if (key[0] == '-')
	{
		*custom_errno = U_INVALID_OPTION;
		return (2);
	}
	if (!find_key(shell, key, &index_of_key))
		return (0);
	return (remove_pair(shell, index_of_key));
}
