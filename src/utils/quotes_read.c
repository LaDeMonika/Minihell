/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_read.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:30:03 by lilin             #+#    #+#             */
/*   Updated: 2024/06/24 18:30:04 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	count_literal_chars(char *str)
{
	char	metaquote;
	int		i;
	int		len;

	metaquote = '\0';
	i = 0;
	len = 0;
	while (str && str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			if (!(metaquote))
				metaquote = str[i];
			else if (metaquote == str[i])
				metaquote = '\0';
			else if (str[i] != metaquote)
				len++;
		}
		else
			len++;
		i++;
	}
	return (len);
}

bool	has_even_metaquotes(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
		{
			i = skip_between_metaquotes(s, i, s[i]);
			if (!s[i])
				return (false);
		}
		i++;
	}
	return (true);
}

int	skip_between_metaquotes(char *str, int i, char metaquote)
{
	i++;
	while (str[i] && str[i] != metaquote)
		i++;
	return (i);
}

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
