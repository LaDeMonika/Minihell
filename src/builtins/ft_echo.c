/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 15:09:56 by msimic            #+#    #+#             */
/*   Updated: 2024/05/31 16:51:20 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../inc/minishell.h"

static void ft_putstr_no_newline(char *str)
{
    while (*str)
    {
        write(1, str, 1);
        str++;
    }
}

bool    contains_only_option_n(char *str)
{
    int i;

    i = 0;
    if (!str || !str[i])
        return (false);
    while (str && str[i])
    {
        while (is_space(str[i]))
            i++;
        if (str[i] == '-')
        {
            i++;
            if (str[i] != 'n')
                return (false);
            while (str[i] == 'n')
                i++;
            if (str[i] && !is_space(str[i]))
                return (false);
        }
        else
            return (false);
    }
    return (true);
}

int ft_echo(char **command_array)
{
    int i = 0;
    int new_line_flag;

    new_line_flag = contains_only_option_n(command_array[i]);
    while (new_line_flag && contains_only_option_n(command_array[i]))
        i++;
    while (command_array[i])
    {
        ft_putstr_no_newline(command_array[i]);
        if (command_array[i + 1])
            write(1, " ", 1);
        i++;
    }
    if (!new_line_flag)
        write(1, "\n", 1);
    return 0;
}
