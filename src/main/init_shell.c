/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 14:26:33 by msimic            #+#    #+#             */
/*   Updated: 2024/05/31 17:00:19 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	init_shell_struct(t_minishell *shell, char **envp)
{
	int	i;
	shell->envp = try_malloc(shell, sizeof(char *) * (sizeof_array((void **)envp) + 1));
	i = 0;
	while (envp[i])
	{
		shell->envp[i] = ft_strdup(shell, envp[i]);
		if (ft_strncmp(envp[i], "_=", 2) == 0)
			update_value(shell, ft_strdup(shell, "_"), "nd", 0);
		i++;
	}

	shell->envp[i] = NULL;
	shell->prompt = NULL;
	shell->usr_input = NULL;
	shell->line_count = 0;
	shell->input_array = NULL;
	shell->pipes_total = 0;
	shell->list = NULL;
	shell->pid = NULL;
	shell->input_file = NULL;
	shell->command_array = NULL;
	shell->path_array = NULL;
	shell->last_exit_status = 0;
	shell->env_subarray = NULL;
}
void	init_input_iteration(t_minishell *shell)
{
	shell->line_count++;
	shell->pipes_total = 0;
	shell->pid = NULL;
	shell->input_file = NULL;
	shell->unexpected_token = NULL;
	shell->stay_in_parent = false;
}

