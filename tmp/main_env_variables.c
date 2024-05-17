#include <stdlib.h>
#include <stdio.h>

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	char			*sub;
	size_t			slen;

	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	if (len > slen - start)
		len = slen - start;
	if (start >= slen)
		len = 0;
	sub = NULL;
	sub = malloc((len + 1) * sizeof(char));
	if (!sub)
		return (NULL);
	sub[len] = '\0';
	i = 0;
	while (i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	return (sub);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		slen;
	char	*s;
	int		i;

	if (!s1 || !s2)
		return (NULL);
	slen = ft_strlen(s1) + ft_strlen(s2);
	s = malloc((slen + 1) * sizeof(char));
	if (!s)
		return (NULL);
	i = 0;
	while (*s1)
	{
		s[i] = *s1;
		i++;
		s1++;
	}
	while (*s2)
	{
		s[i] = *s2;
		i++;
		s2++;
	}
	s[i] = '\0';
	return (s);
}

char	*extract_substr_and_append(char *old_s, int start, int len, char *new_str)
{
	char	*affix;

	printf("new s before join: %s\n", new_str);
	affix = ft_substr(old_s, start, len - start);
	printf("affix: %s\n", affix);
	if (new_str)
		new_str = ft_strjoin(new_str, affix);
	else
		new_str = affix;
	printf("new s after join: %s\n\n", new_str);
	return (new_str);
}

char *check_env_variables(char *s)
{
	int	i;
	int	start;
	char	*env_key;
	char	*env_value;
	char	*new_str;

	i = 0;
	start = 0;
	new_str = NULL;
	while (s[i])
	{
		if (s[i] == '$')
		{
			new_str = extract_substr_and_append(s, start, i, new_str);
			start = i + 1;
			while (s[i] && s[i] != ' ')
				i++;
			printf("start: %d len: %d\n", start, i);
			env_key = ft_substr(s, start, i - start);
			printf("env_key: %s\n", env_key);
			env_value = getenv(env_key);
			printf("env value: %s\n\n", env_value);
			if (env_value)
				new_str = ft_strjoin(new_str, env_value);
			else
			{
			 	new_str = ft_strjoin(new_str, "$");
				new_str = ft_strjoin(new_str, env_key);
			}
			start = i;

		}
		i++;
	}
	if (s[start])
		extract_substr_and_append(s, start, i, new_str);
	return (new_str);
}

int	main(void)
{

	printf("%s\n", check_env_variables("echo \"this $HELLO and $HOME and also $PWD and\""));
	printf("%s\n", "echo \"this $HELLO and $HOME and also $PWD and\"");
}