/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_msg_01.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 14:17:14 by msimic            #+#    #+#             */
/*   Updated: 2024/05/24 14:17:32 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_error(char *prefix, char *custom_message)
{
	write(STDERR_FILENO, prefix, ft_strlen(prefix));
	write(STDERR_FILENO, ": ", 2);
	if (custom_message)
	{
		write(STDERR_FILENO, custom_message, ft_strlen(custom_message));
		write(STDERR_FILENO, "\n", 1);
	}
	else
		perror(NULL);
}
