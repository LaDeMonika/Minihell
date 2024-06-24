/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 15:09:56 by msimic            #+#    #+#             */
/*   Updated: 2024/06/24 18:38:00 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	contains_only_option_n(char *str)
{
	int	i;

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

int	ft_echo(t_minishell *shell)
{
	int	new_line_flag;
	int	i;

	new_line_flag = contains_only_option_n(shell->command_array[1]);
	i = 1;
	while (new_line_flag && contains_only_option_n(shell->command_array[i]))
		i++;
	while (shell->command_array[i])
	{
		printf("%s", shell->command_array[i]);
		if (shell->command_array[i + 1])
			printf(" ");
		i++;
	}
	if (!new_line_flag)
		printf("\n");
	return (0);
}
