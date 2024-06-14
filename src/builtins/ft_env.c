/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:21:57 by msimic            #+#    #+#             */
/*   Updated: 2024/06/11 14:00:09 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	extract_command_and_execute(t_minishell *shell, int start)
{
	int		end;
	int		i;

	end = sizeof_array((void **)shell->command_array);
	shell->env_subarray = try_malloc(shell, sizeof(char *) * (end - start + 1));
	shell->env_subarray = fill_array_with_null(shell->env_subarray, end - start + 1);
	i = 0;
	while (start < end)
	{
		shell->env_subarray[i] = ft_strdup(shell, shell->command_array[start]);
		start++;
		i++;
	}
	free_and_reset_array((void ***)&shell->command_array, false);
	shell->command_array = shell->env_subarray;
    shell->env_subarray = NULL;
	execute_command_array(shell, shell->command_array);
}

int	ft_env(t_minishell *shell, int *custom_errno)
{
	int	i;

	if (shell->command_array[1])
	{
		if (ft_strchr(shell->command_array[1], '='))
		{
			ft_export(shell, shell->command_array[1], custom_errno);
			if (shell->command_array[2])
				extract_command_and_execute(shell, 2);
		}
		else
			extract_command_and_execute(shell, 1);
	}
	else
	{
		i = 0;
		while (shell->envp[i])
		{
			printf("%s\n", shell->envp[i]);
			i++;
		}
	}
	return (0);
}
