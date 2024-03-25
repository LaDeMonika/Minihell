/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 14:39:52 by lilin             #+#    #+#             */
/*   Updated: 2023/09/14 22:26:58 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char	*s)
{
	size_t	ssize;
	char	*new;

	ssize = ft_strlen(s) + 1;
	new = malloc(sizeof(char) * ssize);
	if (!new)
		return (NULL);
	ft_strlcpy(new, s, ssize);
	return (new);
}
