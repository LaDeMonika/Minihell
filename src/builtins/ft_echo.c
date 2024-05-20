/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 15:09:56 by msimic            #+#    #+#             */
/*   Updated: 2024/05/20 17:33:22 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../inc/minishell.h"
#include <stdbool.h>

/*
function for the echo command in bash
handles option -n to print the arguements without a "\n" at the end
*/

//Next to handle:
//1.    echo -n -n Hello World      [should not print -n no matter how many times repeated]
//2.    echo echo -n -n -n Hello World [can print echo and -n as many times there are and prints new line at the end]
//3.    echo hello -n -n hello [should print everything after first hello]
//4.    echo -n "hello hello     hellooo" [should print everything in quotes as it is]
//5.    echo -n hello hello     hellooo [should print every arg separated by one space]

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
    /* printf("string: %s\n", str); */
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
        /* printf("argument: %s\n", command_array[i]); */
        ft_putstr_no_newline(command_array[i]);
        if (command_array[i + 1])
            write(1, " ", 1);
        i++;
    }
    if (!new_line_flag)
        write(1, "\n", 1);
    return 0;
}
