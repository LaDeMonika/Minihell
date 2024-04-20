/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 14:59:33 by lilin             #+#    #+#             */
/*   Updated: 2024/04/20 14:39:16 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		s1len;
	int		s2len;
	char	*s;
	int		i;

	s1len = ft_strlen(s1);
	s2len = ft_strlen(s2);
	s = malloc((s1len + s2len + 1) * sizeof(char));
	if (!s)
		return (NULL);
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
/* int	main(void)
{
	char	*s1;
	char	*s2;
	char	*joined;

	s1 = NULL;
	s2 = NULL;
	printf("before s1: %s s2: %s\n", s1, s2);
	joined = ft_strjoin(s1, s2);
	printf("after joined: %s\n", joined);
}
 */