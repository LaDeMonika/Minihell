/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:21:02 by msimic            #+#    #+#             */
/*   Updated: 2024/05/17 15:30:50 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
function for the cd command in bash
handles the changing in directory and
- symbol => previous directory
~ symbol => goes back to the directory stated in $HOME
.. symbol => goes back one directory
*/

int ft_cd(char **commad_array)
{
    //TODO: we might need to update PWD in env
    int i = 0;
    char *current = getcwd(NULL, 0);

    if (current == NULL)
        printf("Memory allocation failed\n");
    if (ft_strcmp(commad_array[i], "cd") == 0)
    {
        if (ft_strcmp(commad_array[i+1], "~") == 0)
            chdir(getenv("HOME"));
        else if (ft_strcmp(commad_array[i+1], "-") == 0)
            chdir(getenv("OLDPWD"));
        else if (ft_strcmp(commad_array[i+1], "..") == 0)
            chdir(".."); //go behind one directory
        else if (chdir(commad_array[i+1]) == -1)
            printf("There is no such dir: %s\n", commad_array[i+1]);
    }
    return (0);
}