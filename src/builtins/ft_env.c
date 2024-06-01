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



char    **ft_subarray(t_minishell *shell, char **array, int start, int end)
{
    char **subarray;
    int i;

    i = 0;
    subarray = malloc(sizeof(char *) * (end - start + 1));
    while (start < end)
    {
        subarray[i] = ft_strdup(shell, array[start]);
        start++;
        i++;
    }
    subarray[i] = NULL;
    return (subarray);
}

int ft_env(t_minishell *shell, char **command_array)
{
    int i;
    char  **subarray;

    i = 0;
    if (command_array[1])
    {
        if (strchr(command_array[1], '='))
        {
            ft_export(shell, command_array[1]);
            if (command_array[2])
            {
                subarray = ft_subarray(shell, command_array, 1, sizeof_array((void **)command_array));
                free_and_reset_array((void ***)&shell->command_array, false);
                free_and_reset_ptr((void **)&shell->command_array);
                shell->command_array = subarray;
                execute_command_array(shell, shell->command_array);
            }

        }
        else
        {
            subarray = ft_subarray(shell, command_array, 1, sizeof_array((void **)command_array));
            free_and_reset_array((void ***)&shell->command_array, false);
            free_and_reset_ptr((void **)&shell->command_array);
            shell->command_array = subarray;
            execute_command_array(shell, shell->command_array);
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