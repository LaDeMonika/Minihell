/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 14:26:33 by msimic            #+#    #+#             */
/*   Updated: 2024/05/23 16:27:32 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <stdio.h>
#include <unistd.h>

void	init_shell_struct(t_minishell *shell, char **envp)
{
	int	i;
	shell->envp = malloc(sizeof(char *) * (sizeof_array((void **)envp) + 1));
	if (!shell->envp)
		error_free_all(shell, ERR_MALLOC, NULL, NULL);
	/* printf("address of envp at initialization: %p\n", shell->envp); */
	i = 0;
	while (envp[i])
	{
		shell->envp[i] = ft_strdup(shell, envp[i]);
		i++;
	}
	shell->envp[i] = NULL;
	/* printf("final i at initialization: %d\n", i); */
	//shell->envp = envp;

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

