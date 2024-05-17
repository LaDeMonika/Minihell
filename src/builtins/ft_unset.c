/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:23:13 by msimic            #+#    #+#             */
/*   Updated: 2024/05/17 13:23:18 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
The `unset` command is used in Unix-like operating systems to unset or
remove environment variables and shell functions. 
*/
int ft_unset(t_minishell *shell, char **command_array)
{
    //so i should iterate through the env list and remove the variables that are in the command_array
    //if it's iterating ([if] in [while] loop) and char matching one by one and check just till '='; then delete that variable
    //else go to next variable and check if it is that variable
}