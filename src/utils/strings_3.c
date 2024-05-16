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

char	*ft_itoa(t_minishell *shell, int n)
{
	int		len;
	char	*str;

	len = calculate_len(n);
	str = malloc((len + 1) * sizeof(char));
	if (!str)
		return (error_free_all(shell, ERR_MALLOC, NULL, NULL), NULL);
	write_number(n, str, len);
	return (str);
}

bool	has_even_metaquotes(char *s)
{
	int		i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
		{
			skip_between_metaquotes(s, i, s[i]);
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

int	skip_first_metaquote_pair(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
			return (skip_between_metaquotes(str, i, str[i]));
		i++;
	}
	return (0);

}


/*appends suffix to base and frees base afterwards*/
char	*append_suffix(t_minishell *shell, char *base, char *suffix)
{
	char	*new_str;

	new_str = base;
	new_str = ft_strjoin(shell, base, suffix);
	free_and_reset_ptr((void **)&base);
	return (new_str);
}