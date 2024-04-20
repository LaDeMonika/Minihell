/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 15:21:21 by msimic            #+#    #+#             */
/*   Updated: 2024/04/20 14:13:52 by msimic           ###   ########.fr       */
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

/*
ft_is_builtin is a function that will check if the command is a builtin command.
If the command is a builtin command, it will execute the builtin command.
If the command is not a builtin command, it will return 0.
*/
int ft_is_builtin(t_minishell *shell)
{
    int status;

    status = 1; // 1 = builtin, 0 = not builtin
    if (ft_strcmp(shell->input_array[0], "echo") == 0)
        status = ft_echo(shell);
    /* else if (ft_strcmp(shell->input_array[0], "cd") == 0)
        status = ft_cd(shell);
    else if (ft_strcmp(shell->input_array[0], "pwd") == 0)
        status = ft_pwd(shell);
    else if (ft_strcmp(shell->input_array[0], "export") == 0)
        status = ft_export(shell);
    else if (ft_strcmp(shell->input_array[0], "unset") == 0)
        status = ft_unset(shell);
    else if (ft_strcmp(shell->input_array[0], "env") == 0)
        status = ft_env(shell);
    else if (ft_strcmp(shell->input_array[0], "exit") == 0)
        status = ft_exit(shell); */
    if (status == 0)
        shell->exit_status = 0;
    return (status);
}
