/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:37:46 by lilin             #+#    #+#             */
/*   Updated: 2024/06/24 18:37:52 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	ft_exit(t_minishell *shell, int *custom_errno)
{
	bool	is_valid_number;
	int		exit_status;

	printf("exit\n");
	is_valid_number = true;
	if (shell->command_array[1])
	{
		exit_status = ft_atoi(shell->command_array[1], &is_valid_number);
		if (is_valid_number)
		{
			if (shell->command_array[2])
			{
				*custom_errno = U_TOO_MANY_ARGUMENTS;
				return (1);
			}
			error_free_all_exit(shell, exit_status % 256);
		}
		else
		{
			print_error(shell->command_array[1], "numeric argument required");
			error_free_all_exit(shell, 2);
		}
	}
	return (error_free_all_exit(shell, 0), 0);
}
