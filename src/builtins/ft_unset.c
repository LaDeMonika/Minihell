/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:23:13 by msimic            #+#    #+#             */
/*   Updated: 2024/05/31 16:55:54 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int ft_unset(t_minishell *shell, char *key, int *custom_errno)
{
    int i;
    int j;
    int k;
    int old_size;

    i = 0;
    if (key[0] == '-')
    {
        *custom_errno = U_INVALID_OPTION;
        return (2);
    }
    while (shell->envp[i])
    {
        shell->old_key = ft_substr(shell, shell->envp[i], 0,  index_of_char(shell->envp[i], '='));
        if (ft_strcmp(shell->old_key, key) == 0)
        {
            free_and_reset_ptr((void **)&shell->old_key);
            break;
        }
        free_and_reset_ptr((void **)&shell->old_key);
        i++;
    }
    if (!shell->envp[i])
        return (0);
    old_size = sizeof_array((void **)shell->envp);
    shell->new_envp = try_malloc(shell, sizeof(char *) * (old_size));
    j = 0;
    k = 0;
    while (shell->envp[j])
    {
        if (j != i)
        {
            shell->new_envp[k] = shell->envp[j];
            k++;
        }
        else
            free_and_reset_ptr((void **)&shell->envp[j]);
        j++;
    }
    shell->new_envp[k] = NULL;
    free_and_reset_ptr((void **)&shell->envp);
    shell->envp = shell->new_envp;
    shell->new_envp = NULL;
    return (0);
}
