#include "../../inc/minishell.h"

/*extract from old_s a substring and append it to new_s*/
char	*append_substring(char *old_s, int start, int len, char *new_s)
{
	char	*affix;

	affix = ft_substr(old_s, start, len);
	new_s = ft_strjoin(new_s, affix);
	return (new_s);
}

/*if environment value exists to key, then replace it with the value*/
char	*append_env_value(t_minishell *shell, char *old_s, int start, int *i)
{
	char	*env_key;
	char	*env_value;
	char	*new_s;

	env_value = NULL;
	new_s = NULL;
	start = *i + 1;
	while (old_s[*i] && old_s[*i] != ' ' && old_s[*i] != '"')
		(*i)++;
	if (old_s[start] == '?')
		env_value = ft_itoa(shell->last_exit_status);
	else
	{
		env_key = ft_substr(old_s, start, *i - start);
		env_value = getenv(env_key);
	}
	new_s = ft_strjoin(old_s, env_value);
	start = *i;
	return (new_s);

}

/*skip over everything in single quotes and overwrite an environment variable if value exists*/
char	*overwrite_env_variables(t_minishell *shell, char *s)
{
	int	i;
	int	start;
	char	*new_s;


	i = 0;
	start = 0;
	new_s = NULL;
	while (s[i])
	{
		if (s[i] == '\'')
		{
			i++;
			while (s[i] && s[i] != '\'')
				i++;
		}
		else if (s[i] == '$')
		{
			new_s = append_substring(s, start, i - start, new_s);
			new_s = append_env_value(shell, s, start, &i);
		}
		i++;
	}
	if (i != start)
		new_s = append_substring(s, start, i, new_s);
	return (new_s);
}
