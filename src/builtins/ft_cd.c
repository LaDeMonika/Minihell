/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:21:02 by msimic            #+#    #+#             */
/*   Updated: 2024/05/21 17:53:35 by lilin            ###   ########.fr       */
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

int ft_cd(char **command_array)
{
    //TODO: we might need to update PWD in env
    //+ checkout if getcwd really mallocates
    char *current = getcwd(NULL, 0);

    if (current == NULL)
        printf("Memory allocation failed\n");
    if (command_array[1] && command_array[2])
    {
        errno = U_TOO_MANY_ARGUMENTS;
        return (1);
    }

    if (!command_array[1] || ft_strcmp(command_array[1], "--") == 0)
        chdir(getenv("HOME"));
    else if (ft_strcmp(command_array[1], "-") == 0)
        chdir(getenv("OLDPWD"));
    else if (ft_strcmp(command_array[1], "..") == 0)
        chdir(".."); //go behind one directory
    else if (chdir(command_array[1]) == -1)
        return (1);


    return (0);
}