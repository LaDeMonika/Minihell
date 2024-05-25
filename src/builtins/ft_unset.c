/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:23:13 by msimic            #+#    #+#             */
/*   Updated: 2024/05/25 18:38:05 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
Function that will delete the variable from the envp list.
*/


int ft_unset(t_minishell *shell, char *key)
{
    // command_array[0] = unset command_array[1] = variable to unset
    int i;
    int j;
    int k;
    char  *key_in_array;
    int old_size;
    char    **new_envp;



    i = 0;
    while (shell->envp[i])
    {
        key_in_array = ft_substr(shell, shell->envp[i], 0,  index_of_first_occurence(shell->envp[i], '='));
        /* printf("key in array: %s key: %s\n", key_in_array, key); */
        if (ft_strcmp(key_in_array, key) == 0)
            break;
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
            new_envp[k] = ft_strdup(shell, shell->envp[j]);
            /* printf("k: %d j: %d i: %d shell envp: %s new envp: %s\n", k,j, i, shell->envp[j],new_envp[k]); */
            k++;
        }


        j++;

    }
    new_envp[k] = NULL;
    /* printf("k: %d j: %d i: %d shell envp: %s new envp: %s\n", k,j, i, shell->envp[j],new_envp[k]); */
    shell->envp = new_envp;
    return (0);
}
