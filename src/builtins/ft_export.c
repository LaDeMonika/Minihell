/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:22:36 by msimic            #+#    #+#             */
/*   Updated: 2024/05/24 14:22:03 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../inc/minishell.h"
#include <string.h>

int sizeof_array(void **array)
{
    int i;

    i = 0;
    while (array[i])
        i++;
    return (i);
}

int index_of_first_occurence(char *str, char c)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == c)
            return (i);
        i++;
    }
    return (-1);
}

char *update_value(t_minishell *shell, char *arg)
{
    int i;
    char  *name_new;
    char  *name_old;
    char *value;
    char  *new_entry;

    name_new = ft_substr(shell, arg, 0,  index_of_first_occurence(arg, '='));

    i = 0;
    while (shell->envp[i])
    {
        name_old = ft_substr(shell, shell->envp[i], 0,  index_of_first_occurence(shell->envp[i], '='));
        /* printf("name old: %s name new: %s\n", name_old, name_new); */
        if (ft_strcmp(name_new, name_old) == 0)
        {
            value = ft_substr(shell, arg, index_of_first_occurence(arg, '=') + 1, ft_strlen(strchr(arg, '=') - 1));

            new_entry = append_suffix(shell, name_new, "=");
            new_entry = append_suffix(shell, new_entry, value);
           /*  printf("value before: %s\n", shell->envp[i]); */
            shell->envp[i] = ft_strdup(shell, new_entry);

            /* printf("name old: %s value: %s new entry: %s updated entry: %s\n", name_old, value, new_entry, shell->envp[i]); */
            return (shell->envp[i]);
        }
        i++;
    }
    return (NULL);

}


int ft_export(t_minishell *shell, char *arg)
{
     //separete name and value by first =
    //check if name is valid
    //check is there =
    //add name to env
    char **new_envp;
    int old_size;

    int i;



    if (!arg || !strchr(arg, '='))
        return (0);
    if (update_value(shell, arg))
        return (0);
    old_size = sizeof_array((void **)shell->envp);
    new_envp = malloc(sizeof(char *) * (old_size + 2));
    if (!new_envp)
        error_free_all(shell, ERR_MALLOC, NULL, NULL);
    i = 0;
    while (shell->envp[i])
    {
        new_envp[i] = ft_strdup(shell, shell->envp[i]);
        i++;
    }

    new_envp[old_size] = ft_strdup(shell, arg);
    new_envp[old_size + 1] = NULL;
    shell->envp = new_envp;

    return (0);
}