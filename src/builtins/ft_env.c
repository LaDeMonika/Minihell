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

void handle_subarray_extention(t_minishell *shell, char **command_array, int start_index)
{
    char **subarray;

    subarray = ft_subarray(shell, command_array, start_index, sizeof_array((void **)command_array));
    free_and_reset_array((void ***)&shell->command_array, false);
    free_and_reset_ptr((void **)&shell->command_array);
    shell->command_array = subarray;
    execute_command_array(shell, shell->command_array);
}

void export_and_execute(t_minishell *shell, char **command_array, int *custom_errno)
{
    if (ft_strchr(command_array[1], '='))
    {
        ft_export(shell, command_array[1], custom_errno);
        if (command_array[2])
            handle_subarray_extention(shell, command_array, 2);
    }
    else
        handle_subarray_extention(shell, command_array, 1);
}

void print_env(t_minishell *shell)
{
    int i;

    i = 0;
    while (shell->envp[i])
    {
        printf("%s\n", shell->envp[i]);
        i++;
    }
}

int ft_env(t_minishell *shell, char **command_array, int *custom_errno)
{
    if (command_array[1])
        export_and_execute(shell, command_array, custom_errno);
    else
        print_env(shell);
    return (0);
}
