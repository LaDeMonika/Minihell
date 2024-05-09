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
char	*get_env_value(t_minishell *shell, char *old_s, int *start, int *i)
{
	char	*env_key;
	char	*env_value;

	env_key = NULL;
	env_value = NULL;
	*start = *i + 1;
	while (old_s[*i] && old_s[*i] != ' ' && old_s[*i] != '"')
		(*i)++;
	if (old_s[*start] == '?')
		env_value = ft_itoa(shell->last_exit_status);
	else
	{
		env_key = ft_substr(old_s, *start, *i - *start);
		env_value = getenv(env_key);
	}
	*start = *i;
	return (env_value);
}

/*skip over everything in single quotes and overwrite an environment variable if value exists*/
char	*expand_env_variables(t_minishell *shell, char *s)
{
	int		i;
	int		start;
	char	*new_s;
	char	*env_value;

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
			env_value = get_env_value(shell, s, &start, &i);
			new_s = ft_strjoin(new_s, env_value);
		}
		i++;
	}
	if (i != start)
		new_s = append_substring(s, start, i, new_s);
	return (new_s);
}

/*if there was an odd number of quotes,
	this will add a heredoc at the end with first quote as EOF marker*/
char	*append_heredoc_on_missing_quote(t_minishell *shell, char *old_s)
{
	int		i;
	char	quote_type;
	char	*new_s;
	char	closing_quote[2];

	(void)shell;
	new_s = old_s;
	i = 0;
	while (old_s[i])
	{
		if (old_s[i] == '"' || old_s[i] == '\'')
		{
			quote_type = old_s[i];
			i++;
			while (old_s[i] && old_s[i] != quote_type)
				i++;
			if (!old_s[i])
			{
				closing_quote[0] = quote_type;
				closing_quote[1] = '\0';
				new_s = append_to_prompt(shell, old_s, closing_quote);
				//new_s = ft_strjoin(old_s, closing_quote);
				new_s = ft_strjoin(new_s, "<<");
				new_s = ft_strjoin(new_s, closing_quote);
			}
		}
		if (old_s[i])
			i++;
	}
	return (new_s);
}
