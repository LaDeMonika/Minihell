/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:21:02 by msimic            #+#    #+#             */
/*   Updated: 2024/05/25 20:04:12 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <string.h>
#include <sys/types.h>

/*
function for the cd command in bash
handles the changing in directory and
- symbol => previous directory
~ symbol => goes back to the directory stated in $HOME
.. symbol => goes back one directory
*/

int ft_cd(t_minishell *shell, char **command_array)
{
    //TODO: we might need to update PWD in env
    //+ checkout if getcwd really mallocates
    char    *new_pwd;
    char *current_pwd;

    current_pwd = getcwd(NULL, 0);
    if (current_pwd == NULL)
        error_free_all(shell, ERR_GETCWD, NULL, NULL);
    if (command_array[1] && command_array[2])
    {
        free_and_reset_ptr((void **)&current_pwd);

        errno = U_TOO_MANY_ARGUMENTS;
        /* printf("errno: %d should be: %d\n", errno, U_TOO_MANY_ARGUMENTS); */
        return (1);
    }
    if (!command_array[1] || ft_strcmp(command_array[1], "--") == 0 || ft_strcmp(command_array[1], "~") == 0)
    {
        new_pwd = ft_getenv(shell, "HOME");
    }
    else if (command_array[1][0] == '~')
        new_pwd = ft_strjoin(shell, getenv("HOME"), command_array[1] + 1);
    else if (ft_strcmp(command_array[1], "-") == 0)
    {
        new_pwd = ft_getenv(shell, "OLDPWD");
        printf("%s\n", new_pwd);
    }

    else if (ft_strcmp(command_array[1], "..") == 0)
    {
        new_pwd = ft_strdup(shell, ".."); //go behind one directory
    }
    else
        new_pwd = ft_strdup(shell, command_array[1]);

    if (chdir(new_pwd) == -1)
    {
        free_and_reset_ptr((void **)&new_pwd);
        free_and_reset_ptr((void **)&current_pwd);
        return (1);
    }
    free_and_reset_ptr((void **)&new_pwd);
    update_value(shell, ft_strdup(shell, "OLDPWD"),
    current_pwd, 0);
    free_and_reset_ptr((void **)&current_pwd);
    current_pwd = getcwd(NULL, 0);
    update_value(shell, ft_strdup(shell, "PWD"), current_pwd, 0);
    free_and_reset_ptr((void **)&current_pwd);

    return (0);
}