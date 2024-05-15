#include "../../inc/minishell.h"

char	*ft_strchr(const char *s, int c)
{
	while (s && *s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (s && *s == (char)c)
		return ((char *)s);
	return (NULL);
}

char	*ft_strjoin(t_minishell *shell, char const *s1, char const *s2)
{
	int		s1len;
	int		s2len;
	char	*s;
	int		i;

	s1len = ft_strlen(s1);
	s2len = ft_strlen(s2);
	s = malloc((s1len + s2len + 1) * sizeof(char));
	if (!s)
		return (error_free_all(shell, ERR_MALLOC), NULL);
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
char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	if (!big || !little || len < 1)
		return (NULL);
	i = 0;
	if (little[i] == '\0')
		return ((char *)big);
	j = 0;
	while (i < len && big[i])
	{
		if (big[i] == little[j])
		{
			if (!little[j + 1])
				return ((char *)(big + i - j));
			j++;
		}
		else
		{
			i = i - j;
			j = 0;
		}
		i++;
	}
	return (NULL);
}
char	*ft_strtrim(t_minishell *shell, char const	*s1, char const	*set)
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
		return (error_free_all(shell, ERR_MALLOC), NULL);
	ft_strlcpy(new, s1 + start, newlen + 1);
	*(new + newlen) = '\0';
	return (new);
}
