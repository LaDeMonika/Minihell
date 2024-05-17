/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:22:55 by msimic            #+#    #+#             */
/*   Updated: 2024/05/17 13:22:59 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
pwd prints the current working directory
*/

int ft_pwd(char **command_array)
{
    int i = 0;

    if (ft_strcmp(command_array[i], "pwd") == 0)
    {
        char *current = getcwd(NULL, 0);
        if (current == NULL)
            printf("Memory allocation failed\n");
        else
            printf("%s\n", current);
    }
    return (0);
}