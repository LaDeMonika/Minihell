/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_string.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:30:09 by lilin             #+#    #+#             */
/*   Updated: 2024/06/24 18:30:10 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*ft_strjoin(t_minishell *shell, char const *s1, char const *s2)
{
	int		s1len;
	int		s2len;
	char	*s;
	int		i;

	s1len = ft_strlen(s1);
	s2len = ft_strlen(s2);
	s = try_malloc(shell, (s1len + s2len + 1) * sizeof(char));
	i = 0;
	while (s1 && *s1)
	{
		s[i++] = *s1;
		s1++;
	}
	while (s2 && *s2)
	{
		s[i++] = *s2;
		s2++;
	}
	s[i] = '\0';
	return (s);
}

char	*append(t_minishell *shell, char *base, char *suffix,
		int free_afterwards)
{
	char	*new_str;

	new_str = ft_strjoin(shell, base, suffix);
	if (free_afterwards == BASE || free_afterwards == BOTH)
		free_and_reset_ptr((void **)&base);
	if (free_afterwards == SUFFIX || free_afterwards == BOTH)
		free_and_reset_ptr((void **)&suffix);
	return (new_str);
}

char	*ft_substr(t_minishell *shell, char const *s, unsigned int start,
		size_t len)
{
	unsigned int	i;
	char			*sub;
	size_t			slen;

	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	if (len > slen - start)
		len = slen - start;
	if (start >= slen)
		len = 0;
	sub = NULL;
	sub = try_malloc(shell, (len + 1) * sizeof(char));
	sub[len] = '\0';
	i = 0;
	while (i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	return (sub);
}

char	*ft_strdup(t_minishell *shell, const char *s)
{
	size_t	ssize;
	char	*new;

	if (!s)
		return (NULL);
	ssize = ft_strlen(s) + 1;
	new = try_malloc(shell, sizeof(char) * ssize);
	ft_strlcpy(new, s, ssize);
	return (new);
}
