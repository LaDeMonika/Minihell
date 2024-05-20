/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 15:09:56 by msimic            #+#    #+#             */
/*   Updated: 2024/05/20 14:15:46 by lilin            ###   ########.fr       */
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

static void ft_putstr_no_newline(char *str, int flag)
{
    (void)flag;
    while (*str)
    {
        write(1, str, 1);
        str++;
    }
}

bool    contains_only_char(char *str, char c)
{
    while (str && *str)
    {
        if (*str != c)
            return (false);
        str++;
    }
    return (true);
}

int ft_echo(char **command_array)
{
    int i = 0;
    int new_line_flag = 0;

    while (command_array[i])
    {
        /* printf("argument: %s\n", command_array[i]); */
        if (i == 0 && ft_strcmp(command_array[i], "echo") == 0)
        {
            i++;
            if (!command_array[i])
                break;
            if (ft_strncmp(command_array[i], "-n", 2) == 0)
            {
                if (contains_only_char(command_array[i] + 2, 'n'))
                {
                    new_line_flag = 1;
                    i++;
                }
            }
        }
        if (!command_array[i])
            break;
        ft_putstr_no_newline(command_array[i], new_line_flag);
        if (command_array[i + 1])
            write(1, " ", 1);
        i++;
    }
    if (new_line_flag == 0)
        write(1, "\n", 1);
    return 0;
}
