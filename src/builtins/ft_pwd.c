/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:22:55 by msimic            #+#    #+#             */
/*   Updated: 2024/05/24 16:17:43 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
pwd prints the current working directory
*/

int ft_pwd()
{

    char *current = getcwd(NULL, 0);
    if (current == NULL)
        printf("Memory allocation failed\n");
    else
        printf("%s\n", current);
    return (0);
}