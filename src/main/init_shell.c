/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 14:26:33 by msimic            #+#    #+#             */
/*   Updated: 2024/05/10 13:22:10 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <stdio.h>
#include <unistd.h>

void	init_shell_struct(t_minishell *shell, char **envp)
{
	shell->envp = envp;
	shell->prompt = NULL;
	shell->usr_input = NULL;
	shell->line_count = 0;
	shell->input_array = NULL;
	shell->pipes_total = 0;
	shell->list = NULL;
	shell->pid = NULL;
	shell->input_file = NULL;
}
