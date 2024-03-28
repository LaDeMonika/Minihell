/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 15:26:22 by lilin             #+#    #+#             */
/*   Updated: 2024/03/27 16:51:12 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const	*s1, char const	*set)
{
	int		start;
	int		end;
	char	*new;
	int		newlen;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	while (ft_strchr(set, s1[start]))
		start++;
	end = ft_strlen(s1) - 1;
	while (ft_strchr(set, s1[end]))
		end--;
	newlen = end - start + 1;
	if (newlen < 0)
		newlen = 0;
	new = malloc((newlen + 1) * sizeof(char));
	if (!new)
		return (NULL);
	ft_strlcpy(new, s1 + start, newlen + 1);
	*(new + newlen) = '\0';
	return (new);
}
