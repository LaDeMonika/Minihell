/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:30:05 by lilin             #+#    #+#             */
/*   Updated: 2024/06/24 18:30:06 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**fill_array_with_null(char **array, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		array[i] = NULL;
		i++;
	}
	return (array);
}

int	sizeof_array(void **array)
{
	int	i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}
