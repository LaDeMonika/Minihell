/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:21:02 by msimic            #+#    #+#             */
/*   Updated: 2024/06/11 12:38:53 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int handle_to_many_args(char **current_pwd, int *custom_errno)
{
    free_and_reset_ptr((void **)current_pwd);
    *custom_errno = U_TOO_MANY_ARGUMENTS;
    return (1);
}

char *get_new_pwd(t_minishell *shell, char **command_array)
{
    char *new_pwd;

    if (!command_array[1] || ft_strcmp(command_array[1], "--") == 0 || ft_strcmp(command_array[1], "~") == 0)
        new_pwd = ft_getenv(shell, "HOME");
    else if (command_array[1][0] == '~')
        new_pwd = ft_strjoin(shell, getenv("HOME"), command_array[1] + 1);
    else if (ft_strcmp(command_array[1], "-") == 0)
    {
        new_pwd = ft_getenv(shell, "OLDPWD");
        printf("%s\n", new_pwd);
    }
    else if (ft_strcmp(command_array[1], "..") == 0)
        new_pwd = ft_strdup(shell, "..");
    else
        new_pwd = ft_strdup(shell, command_array[1]);
    return (new_pwd);
}




int ft_cd(t_minishell *shell, char **command_array, int *custom_errno)
{
    char    *new_pwd;
    char *current_pwd;

    current_pwd = getcwd(NULL, 0);
    if (current_pwd == NULL)
        error_free_all(shell, ERR_GETCWD, NULL, NULL);
    if (command_array[1] && command_array[2])
        return (handle_to_many_args(&current_pwd, custom_errno));
    new_pwd = get_new_pwd(shell, command_array);
    if (chdir(new_pwd) == -1)
    {
        if (access(new_pwd, F_OK) == -1)
            *custom_errno = U_NO_FILE;
        else if (access(new_pwd, R_OK) == -1)
            *custom_errno = U_NO_PERMISSION;
        /* printf("new_pwd: %s\n", new_pwd);
        printf("errno: %d\n", errno); */
        free_and_reset_ptr((void **)&new_pwd);
        free_and_reset_ptr((void **)&current_pwd);

        /* *custom_errno = U_CD_ERROR; */
        return (1);
    }
    free_and_reset_ptr((void **)&new_pwd);
    update_value(shell, ft_strdup(shell, "OLDPWD"), current_pwd, false);
    free_and_reset_ptr((void **)&current_pwd);
    current_pwd = getcwd(NULL, 0);
    update_value(shell, ft_strdup(shell, "PWD"), current_pwd, false);
    free_and_reset_ptr((void **)&current_pwd);
    return (0);
}
