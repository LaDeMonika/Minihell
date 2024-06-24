/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_string.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 14:20:45 by msimic            #+#    #+#             */
/*   Updated: 2024/06/24 18:30:17 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	ft_atoi(const char *nptr, bool *is_valid_number)
{
	int	i;
	int	sign;
	int	result;
	int	digit;

	i = 0;
	sign = 1;
	result = 0;
	while (is_space(nptr[i]))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	if (nptr[i] < '0' || nptr[i] > '9')
		*is_valid_number = false;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		digit = nptr[i] - '0';
		result = result * 10 + digit;
		i++;
	}
	return (result * sign);
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

char	*ft_strtrim(t_minishell *shell, char const *s1, char const *set)
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
	if (ft_strlen(s1) == 0 || start > end)
		return (free_and_reset_ptr((void **)&s1), ft_strdup(shell, ""));
	while (ft_strchr(set, s1[end]))
		end--;
	newlen = end - start + 1;
	new = try_malloc(shell, (newlen + 1) * sizeof(char));
	ft_strlcpy(new, s1 + start, newlen + 1);
	*(new + newlen) = '\0';
	free_and_reset_ptr((void **)&s1);
	return (new);
}
