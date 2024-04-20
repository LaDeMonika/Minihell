/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 14:39:52 by lilin             #+#    #+#             */
/*   Updated: 2024/04/20 16:11:39 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char	*s)
{
	size_t	ssize;
	char	*new;

	if (!s)
		return (NULL);
	ssize = ft_strlen(s) + 1;
	new = malloc(sizeof(char) * ssize);
	if (!new)
		return (NULL);
	ft_strlcpy(new, s, ssize);
	return (new);
}
