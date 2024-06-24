/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_string_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:29:12 by lilin             #+#    #+#             */
/*   Updated: 2024/06/24 18:29:13 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*ft_strchr(const char *s, char c)
{
	while (s && c && *s)
	{
		if (*s == c)
			return ((char *)s);
		s++;
	}
	if (s && c && *s == c)
		return ((char *)s);
	return (NULL);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	if (!big || !little || len < 1)
		return (NULL);
	i = 0;
	if (little[i] == '\0')
		return ((char *)big);
	j = 0;
	while (i < len && big[i])
	{
		if (big[i] == little[j])
		{
			if (!little[j + 1])
				return ((char *)(big + i - j));
			j++;
		}
		else
		{
			i = i - j;
			j = 0;
		}
		i++;
	}
	return (NULL);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (!s1 && !s2)
		return (0);
	if (!s1 || !s2)
		return (1);
	while (i < n)
	{
		if (!s1[i] || !s2[i] || s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 && !s2)
		return (0);
	if (!s1 || !s2)
		return (1);
	while (s1[i] == s2[i] && s1[i] && s2[i])
		i++;
	if (s1[i] != s2[i])
		return (1);
	return (s1[i] - s2[i]);
}
