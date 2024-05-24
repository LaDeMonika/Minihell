/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:21:02 by msimic            #+#    #+#             */
/*   Updated: 2024/05/24 16:45:39 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
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
        printf("Memory allocation failed\n");
    if (command_array[1] && command_array[2])
    {
        errno = U_TOO_MANY_ARGUMENTS;
        return (1);
    }
    ;
    if (!command_array[1] || ft_strcmp(command_array[1], "--") == 0 || ft_strcmp(command_array[1], "~") == 0)
    {
        new_pwd = getenv("HOME");
    }

    else if (ft_strcmp(command_array[1], "-") == 0)
    {
        new_pwd = ft_getenv(shell, "OLDPWD");
        printf("%s\n", new_pwd);
    }

    else if (ft_strcmp(command_array[1], "..") == 0)
    {
        new_pwd = ".."; //go behind one directory
    }
    else
        new_pwd = command_array[1];

    if (chdir(new_pwd) == -1)
        return (1);
    update_value(shell, ft_strdup(shell, "OLDPWD"), current_pwd);

    return (0);
}