/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings_5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 14:25:21 by msimic            #+#    #+#             */
/*   Updated: 2024/05/24 14:27:30 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	skip_first_metaquote_pair(char *str)
{
	int	i;
	int	new_i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			new_i = skip_between_metaquotes(str, i, str[i]);
			if (str[new_i])
				return (new_i);
			break ;
		}
		i++;
	}
	return (0);
}

char	*append_suffix(t_minishell *shell, char *base, char *suffix)
{
	char	*new_str;

	new_str = base;
	new_str = ft_strjoin(shell, base, suffix);
	free_and_reset_ptr((void **)&base);
	return (new_str);
}
