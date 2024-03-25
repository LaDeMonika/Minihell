/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 16:24:47 by lilin             #+#    #+#             */
/*   Updated: 2023/09/14 22:30:33 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	calculate_len(int n)
{
	int	len;

	len = 0;
	if (n == INT_MIN)
		return (11);
	else if (n == 0)
		return (1);
	else if (n < 0)
		len++;
	while (n != 0)
	{
		len++;
		n /= 10;
	}
	return (len);
}

static void	write_number(int n, char	*str, int len)
{
	char	digit;

	str[len] = '\0';
	if (n == INT_MIN)
		ft_strlcpy(str, "-2147483648", len +1);
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

char	*ft_itoa(int n)
{
	int		len;
	char	*str;

	len = calculate_len(n);
	str = malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	write_number(n, str, len);
	return (str);
}
