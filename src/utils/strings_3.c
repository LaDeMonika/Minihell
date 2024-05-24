/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 14:20:42 by msimic            #+#    #+#             */
/*   Updated: 2024/05/24 14:25:34 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

static void	write_number(int n, char *str, int len)
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

char	*ft_itoa(t_minishell *shell, int n)
{
	int		len;
	char	*str;

	len = calculate_len(n);
	str = try_malloc(shell, (len + 1) * sizeof(char));
	write_number(n, str, len);
	return (str);
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
