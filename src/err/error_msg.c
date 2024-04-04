/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_msg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 12:57:17 by msimic            #+#    #+#             */
/*   Updated: 2024/04/04 16:31:14 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void    ft_puterror(const char *fault, const char *msg)
{
    if (fault && ft_strlen(fault))
        printf("Error: %s\n", fault);
    if (msg && ft_strlen(msg))
        printf("Expected: %s\n", msg);
    printf("\n");
}

void    ft_error_msg(char err)
{
    if (err == ERR_MALLOC)
        ft_puterror("Memory allocation failed", "");
    else if (err == ERR_TOO_MANY_ARGS)
        ft_puterror("Too many arguments", "Please try again");
    else if (err == ERR_TOO_FEW_ARGS)
        ft_puterror("Too few arguments", "Please try again");
    else if (err == ERR_INVALID_ARG)
        ft_puterror("Invalid argument", "Please try again");
    else if (err == ERR_PATH_NOT_FOUND)
        ft_puterror("Path not found", "");
}