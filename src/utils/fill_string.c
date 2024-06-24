/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_string.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:28:31 by lilin             #+#    #+#             */
/*   Updated: 2024/06/24 18:28:32 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	write_number(int n, char *str, int len)
{
	char	digit;

	str[len] = '\0';
	if (n == INT_MIN)
		ft_strlcpy(str, "-2147483648", len + 1);
	else if (n == 0)
		str[0] = '0';
	else if (n < 0)
	{
		str[0] = '-';
		n *= (-1);
	}
	while (n > 0)
	{
		digit = n % 10 + '0';
		str[len - 1] = digit;
		n /= 10;
		len--;
	}
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	srclen;

	srclen = 0;
	if (!src || !dst || size < 1)
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

void	copy_char_and_increment_new_str(char *new_str, char c, int *j)
{
	new_str[*j] = c;
	(*j)++;
}
