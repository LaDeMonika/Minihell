/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:23:13 by msimic            #+#    #+#             */
/*   Updated: 2024/05/20 16:44:40 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
Function that will delete the variable from the envp list.
And return the new envp list.
*/
char **delete_env(t_minishell *shell, int index)
{
    int i = 0;
    int j = 0;
    char **new_envp = NULL;
    
    while (shell->envp[i])
        i++;
    new_envp = malloc(sizeof(char *) * i);
    if (!new_envp)
        return (NULL);
    i = 0;
    while (shell->envp[i])
    {
        if (i == index)
        {
            new_envp[j] = shell->envp[i];
            j++;
        }
        i++;
    }
    new_envp[j] = NULL;
    free(shell->envp);
    return (new_envp);
}

int ft_unset(t_minishell *shell, char **command_array)
{
    // command_array[0] = unset command_array[1] = variable to unset
    int i = 0;
    int j = 0;
    char *env_name = NULL;
    (void)shell;
    char *name = command_array[1];

    if (!name)
        return (0);

    printf("ft_unset\n");
    while (shell->envp[i])
    {
        env_name = strtok(shell->envp[i], "="); //ft_split
        if (ft_strcmp(env_name, name) == 0)
        {
            j = i;
            printf("unset: %s\n", shell->envp[j]); //test
            while (shell->envp[j])
            {
                shell->envp[j] = shell->envp[j + 1];
                j++;
            }
            break;
        }
        i++;
    }
    shell->envp = delete_env(shell, i);
    printf("new envp: %s\n", shell->envp[i]);
    return (0);
}
