/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:21:57 by msimic            #+#    #+#             */
/*   Updated: 2024/05/31 16:51:44 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int ft_env(t_minishell *shell, char **command_array)
{
    int i;

    i = 0;
    if (command_array[1])
    {
        if (strchr(command_array[1], '='))
        {
            ft_export(shell, command_array[1]);
            if (command_array[2])
            {
                shell->command_array = command_array + 2;
                execute_command_array(shell, command_array + 2);
            }
        }
        else
        {
            shell->command_array = command_array + 1;
            execute_command_array(shell, command_array + 1);
        }

    }
    else
    {
        while (shell->envp[i])
        {
            printf("%s\n", shell->envp[i]);
            i++;
        }
    }
    return (0);
}