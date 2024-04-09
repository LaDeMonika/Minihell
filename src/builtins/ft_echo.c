/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 15:09:56 by msimic            #+#    #+#             */
/*   Updated: 2024/04/09 14:22:33 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
ft_echo is a function that will print the arguments to the standard output.
*/
int ft_echo(t_minishell *shell)
{
    int i;

    i = 1;
    while (shell->input_array[i])
    {
        ft_putstr_fd(shell->input_array[i], 1); // Print the argument
        if (shell->input_array[i + 1])
            ft_putstr_fd(" ", 1); // Print a space if there is another argument
        i++;
    }
    ft_putstr_fd("\n", 1);
    return (1);
}