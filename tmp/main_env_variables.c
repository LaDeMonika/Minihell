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

char	*append_affix(char *old_s, int start, int len, char *new_s)
{
	char	*affix;

	printf("new s before join: %s\n", new_s);
	affix = ft_substr(old_s, start, len - start);
	printf("affix: %s\n", affix);
	if (new_s)
		new_s = ft_strjoin(new_s, affix);
	else
		new_s = affix;
	printf("new s after join: %s\n\n", new_s);
	return (new_s);
}

char *check_env_variables(char *s)
{
	int	i;
	int	start;
	char	*env_key;
	char	*env_value;
	char	*new_s;

	i = 0;
	start = 0;
	new_s = NULL;
	while (s[i])
	{
		if (s[i] == '$')
		{
			new_s = append_affix(s, start, i, new_s);
			start = i + 1;
			while (s[i] && s[i] != ' ')
				i++;
			printf("start: %d len: %d\n", start, i);
			env_key = ft_substr(s, start, i - start);
			printf("env_key: %s\n", env_key);
			env_value = getenv(env_key);
			printf("env value: %s\n\n", env_value);
			if (env_value)
				new_s = ft_strjoin(new_s, env_value);
			else
			{
			 	new_s = ft_strjoin(new_s, "$");
				new_s = ft_strjoin(new_s, env_key);
			}
			start = i;

		}
		i++;
	}
	if (s[start])
		append_affix(s, start, i, new_s);
	return (new_s);
}

int	main(void)
{

	printf("%s\n", check_env_variables("echo \"this $HELLO and $HOME and also $PWD and\""));
	printf("%s\n", "echo \"this $HELLO and $HOME and also $PWD and\"");
}