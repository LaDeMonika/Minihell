/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:22:36 by msimic            #+#    #+#             */
/*   Updated: 2024/06/11 15:25:33 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"


char *update_value(t_minishell *shell, char *key, char *value, bool to_append)
{
    int i;


    i = 0;

    while (shell->envp[i])
    {
        shell->old_key = ft_substr(shell, shell->envp[i], 0,  first_occurence_of_char(shell->envp[i], '='));
        if (ft_strcmp(shell->old_key, key) == 0)
        {
            shell->new_entry = append(shell, key, "=", FREE_NONE);
            if (to_append)
            {
                shell->old_value = ft_substr(shell, shell->envp[i], first_occurence_of_char(shell->envp[i], '=') + 1, ft_strlen(strchr(shell->envp[i], '=') - 1));
                shell->new_entry = append(shell, shell->new_entry, shell->old_value, FREE_BOTH);
            }
            shell->new_entry = append(shell, shell->new_entry, value, FREE_BASE);
            free_and_reset_ptr((void **)&shell->envp[i]);
            shell->envp[i] = shell->new_entry;
            shell->new_entry = NULL;
            free_and_reset_ptr((void **)&shell->old_key);
            return (shell->envp[i]);
        }
        free_and_reset_ptr((void **)&shell->old_key);
        i++;
    }
    return (NULL);
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

bool    parse_check_export_args(char *arg, int *custom_errno, int *exit_code)
{
    if (arg[0] == '-')
    {
        *custom_errno = U_INVALID_OPTION;
        *exit_code = 2;
        return (false);
    }
    if (!valid_arg(arg))
    {
        *custom_errno = U_INVALID_IDENTIFIER;
        *exit_code = 1;
        return (false);
    }
    if (count_occurences_of_char(arg, '=') < 1)
    {
        *exit_code = 0;
        return (false);
    }
    return (true);
}

int ft_export(t_minishell *shell, char *arg, int *custom_errno)
{
    int old_size;
    int    to_append;
    int i;
    int exit_code;

    if (!parse_check_export_args(arg, custom_errno, &exit_code))
        return (exit_code);

    to_append = 0;
    if (ft_strnstr(arg, "+=", ft_strlen(arg)))
        to_append = 1;
    shell->new_key = ft_substr(shell, arg, 0,  first_occurence_of_char(arg, '=') - to_append);
    shell->new_value = ft_substr(shell, arg, first_occurence_of_char(arg, '=') + 1, ft_strlen(strchr(arg, '=') - 1));
    if (update_value(shell, shell->new_key, shell->new_value, to_append))
    {
        free_and_reset_ptr((void **)&shell->new_key);
        free_and_reset_ptr((void **)&shell->new_value);
        return (0);
    }
    old_size = sizeof_array((void **)shell->envp);
    shell->new_envp = try_malloc(shell, sizeof(char *) * (old_size + 2));
    shell->new_envp = fill_array_with_null(shell->new_envp, old_size + 2);
    i = 0;
    while (shell->envp[i])
    {
        shell->new_envp[i] = shell->envp[i];
        i++;
    }
    if (to_append)
    {
        shell->new_entry = ft_strjoin(shell, shell->new_key, "=");
        shell->new_entry = append(shell, shell->new_entry, shell->new_value, FREE_BASE);
    }
    else
        shell->new_entry = ft_strdup(shell, arg);
    shell->new_envp[old_size] = shell->new_entry;
    shell->new_entry = NULL;
    shell->new_envp[old_size + 1] = NULL;
    free_and_reset_ptr((void **)&shell->envp);
    shell->envp = shell->new_envp;
    shell->new_envp = NULL;
    free_and_reset_ptr((void **)&shell->new_key);
    free_and_reset_ptr((void **)&shell->new_value);

    return (0);
}

int export_no_args(t_minishell  *shell)
{
    int i;

    i = 0;
    while (shell->envp[i])
    {
        shell->old_key = ft_substr(shell, shell->envp[i], 0,  first_occurence_of_char(shell->envp[i], '='));
        shell->old_value = ft_substr(shell, shell->envp[i], first_occurence_of_char(shell->envp[i], '=') + 1, ft_strlen(strchr(shell->envp[i], '=') - 1));
        printf("declare -x %s=\"%s\"\n", shell->old_key, shell->old_value);
        free_and_reset_ptr((void **)&shell->old_key);
        free_and_reset_ptr((void **)&shell->old_value);
        i++;
    }
    return (0);
}
