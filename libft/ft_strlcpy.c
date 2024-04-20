/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 15:54:09 by lilin             #+#    #+#             */
/*   Updated: 2024/04/20 16:44:46 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char	*dst, const char	*src, size_t	size)
{
	size_t	srclen;

	srclen = 0;
	if (!src || ! dst || size < 1)
		return (srclen);
	while (src[srclen] && (srclen + 1 < size))
	{
		dst[srclen] = src[srclen];
		srclen++;
	}
	if (size > 0)
		dst[srclen] = '\0';
	while (src[srclen])
		srclen++;
	return (srclen);
}
