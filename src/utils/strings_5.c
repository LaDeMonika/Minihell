/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings_5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 14:25:21 by msimic            #+#    #+#             */
/*   Updated: 2024/05/25 18:17:03 by lilin            ###   ########.fr       */
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

char	*append(t_minishell *shell, char *base, char *suffix, int free_afterwards)
{
	char	*new_str;

	new_str = ft_strjoin(shell, base, suffix);
	if (free_afterwards == BASE || free_afterwards == BOTH)
		free_and_reset_ptr((void **)&base);
	if (free_afterwards == SUFFIX || free_afterwards == BOTH)
		free_and_reset_ptr((void **)&suffix);
	return (new_str);
}

bool    ft_isalpha(char c)
{
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        return (true);
    return (false);
}

int sizeof_array(void **array)
{
    int i;

    i = 0;
    while (array[i])
        i++;
    return (i);
}