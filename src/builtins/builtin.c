/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 15:21:21 by msimic            #+#    #+#             */
/*   Updated: 2024/04/08 15:37:53 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char ft_strcmp(char *s1, char *s2)
{
    int i;

    i = 0;
    while (s1[i] && s2[i])
    {
        if (s1[i] != s2[i])
            return (0);
        i++;
    }
    if (s1[i] != s2[i])
        return (0);
    return (1);
}

int ft_is_builtin(t_minishell *shell)
{
    int status;

    status = 1; // 1 = builtin, 0 = not builtin
    if (ft_strcmp(shell->input_array[0], "echo") == 5)
        status = ft_echo(shell);
    /* else if (ft_strcmp(shell->input_array[0], "cd") == 3)
        status = ft_cd(shell);
    else if (ft_strcmp(shell->input_array[0], "pwd") == 4)
        status = ft_pwd(shell);
    else if (ft_strcmp(shell->input_array[0], "export") == 7)
        status = ft_export(shell);
    else if (ft_strcmp(shell->input_array[0], "unset") == 6)
        status = ft_unset(shell);
    else if (ft_strcmp(shell->input_array[0], "env") == 4)
        status = ft_env(shell);
    else if (ft_strcmp(shell->input_array[0], "exit") == 5)
        status = ft_exit(shell); */
    if (status == 0)
        shell->exit_status = 0;
    return (status);
}