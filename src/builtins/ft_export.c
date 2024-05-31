/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:22:36 by msimic            #+#    #+#             */
/*   Updated: 2024/05/25 18:16:59 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../inc/minishell.h"
#include <stdbool.h>
#include <string.h>



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

char *update_value(t_minishell *shell, char *key, char *value, bool append)
{
    int i;

    char  *key_in_array;
    char *value_in_array;
    char  *new_entry;



    i = 0;
    /* printf("update check envp: %s\n", shell->envp[0]); */
    while (shell->envp[i])
    {
        /* printf("current i in update value: %d\n", i); */
        key_in_array = ft_substr(shell, shell->envp[i], 0,  index_of_first_occurence(shell->envp[i], '='));

        /* printf("key in array: %s key: %s\n", key_in_array, key); */
        if (ft_strcmp(key_in_array, key) == 0)
        {


            new_entry = append_suffix(shell, key, "=");
            if (append)
            {
                value_in_array = ft_substr(shell, shell->envp[i], index_of_first_occurence(shell->envp[i], '=') + 1, ft_strlen(strchr(shell->envp[i], '=') - 1));
            }
            new_entry = append_suffix(shell, new_entry, value);
            /* printf("value before: %s\n", shell->envp[i]); */
            shell->envp[i] = ft_strdup(shell, new_entry);

            /* printf("key: %s value: %s new entry: %s updated entry: %s\n", key, value, new_entry, shell->envp[i]); */
            free_and_reset_ptr((void **)&key_in_array);
            return (shell->envp[i]);
        }
        free_and_reset_ptr((void **)&key_in_array);
        i++;
    }
    return (NULL);

}
int count_occurences_of_char(char *str, char c)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while (str[i])
    {
        if (str[i] == c)
            count++;
        i++;
    }
    return (count);
}



bool    valid_arg(char *str)
{
    int i;

    if (!ft_isalpha(str[0]) && str[0] != '_')
        return (false);
    i = 0;
    while (str[i] && str[i] != '=' && str[i] != '+')
    {
        if (!ft_isalnum(str[i]) && str[i] != '_' && str[i])
            return (false);
        i++;
    }

    if (str[i] == '+' && str[i + 1] != '=')
        return (false);
    return (true);
}

int ft_export(t_minishell *shell, char *arg)
{
     //separete name and value by first =
    //check if name is valid
    //check is there =
    //add name to env
    char  *key;
    char    *value;
    char **new_envp;
    int old_size;
    int    append;
    char    *new_entry;


    int i;



    if (!valid_arg(arg))
    {
        errno = U_INVALID_IDENTIFIER;
        return(1);
    }
    if (count_occurences_of_char(arg, '=') < 1)
        return (0);
    append = 0;
    if (ft_strnstr(arg, "+=", ft_strlen(arg)))
        append = 1;

    key = ft_substr(shell, arg, 0,  index_of_first_occurence(arg, '=') - append);
    /* printf("append: %d key: %s\n", append, key); */
    value = ft_substr(shell, arg, index_of_first_occurence(arg, '=') + 1, ft_strlen(strchr(arg, '=') - 1));


    if (update_value(shell, key, value, append))
        return (0);
    old_size = sizeof_array((void **)shell->envp);
    new_envp = malloc(sizeof(char *) * (old_size + 2));
    if (!new_envp)
        error_free_all(shell, ERR_MALLOC, NULL, NULL);
    i = 0;
    while (shell->envp[i])
    {
        new_envp[i] = shell->envp[i];
        i++;
    }
    if (append)
    {
        new_entry = append_suffix(shell, ft_strdup(shell, key), "=");
        new_entry = append_suffix(shell, new_entry, value);
    }
    else
        new_entry = ft_strdup(shell, arg);
    /* printf("check\n"); */
    new_envp[old_size] = new_entry;
    new_envp[old_size + 1] = NULL;
    free_and_reset_ptr((void **)&shell->envp);
    shell->envp = new_envp;
    free_and_reset_ptr((void **)&key);
    free_and_reset_ptr((void **)&value);
    return (0);
}
