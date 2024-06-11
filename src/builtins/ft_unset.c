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
    char  *key_in_array;
    int old_size;
    char    **new_envp;

    i = 0;
    if (key[0] == '-')
    {
        *custom_errno = U_INVALID_OPTION;
        return (2);
    }
    while (shell->envp[i])
    {
        key_in_array = ft_substr(shell, shell->envp[i], 0,  index_of_first_occurence(shell->envp[i], '='));
        if (ft_strcmp(key_in_array, key) == 0)
        {
            free_and_reset_ptr((void **)&key_in_array);
            break;
        }
        free_and_reset_ptr((void **)&key_in_array);
        i++;
    }
    if (!shell->envp[i])
        return (0);
    old_size = sizeof_array((void **)shell->envp);
    new_envp = malloc(sizeof(char *) * (old_size));
    if (!new_envp)
        error_free_all(shell, ERR_MALLOC, NULL, NULL);
    j = 0;
    k = 0;
    while (shell->envp[j])
    {
        if (j != i)
        {
            new_envp[k] = shell->envp[j];
            k++;
        }
        else
            free_and_reset_ptr((void **)&shell->envp[j]);
        j++;
    }
    new_envp[k] = NULL;
    free_and_reset_ptr((void **)&shell->envp);
    shell->envp = new_envp;
    return (0);
}
