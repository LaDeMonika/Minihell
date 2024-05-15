#include "../../inc/minishell.h"

char	*ft_getpid(t_minishell *shell)
{
	int		fd;
	char	buffer[1];
	char	*pid;
	int		i;

	pid = NULL;
	pid = malloc(sizeof(char) * 11);
	if (!pid)
		return (error_free_all(shell, ERR_MALLOC), NULL);
	i = 0;
	fd = open("/proc/self/stat", O_RDONLY);
	if (fd > 0)
	{
		while (read(fd, buffer, 1) > 0)
		{
			if (buffer[0] != ' ')
			{
				pid[i] = buffer[0];
				i++;
			}
			else
			{
				pid[i] = '\0';
				close(fd);
				return (pid);
			}
		}
	}
	else
		return (free(pid), error_free_all(shell, ERR_OPEN), NULL);
	close(fd);
	return (free(pid), error_free_all(shell, ERR_GETPID), NULL);
}

/*if environment value exists to key, then replace it with the value
returns mallocated string that has to be freed*/
char	*get_env_value(t_minishell *shell, char *base, int *start, int *i)
{
	char	*env_key;
	char	*env_value;

	env_key = NULL;
	env_value = NULL;
	(*i)++;
	if (ft_isalnum(base[*i]))
	{
		*start = *i;
		while (ft_isalnum(base[*i]))
			(*i)++;
		env_key = ft_substr(shell, base, *start, *i - *start);
		env_value = ft_strdup(shell, getenv(env_key));
		free(env_key);
		*start = *i;
		(*i)--;
	}
	else
	{
		*start = *i + 1;
		if (!base[*i] || base[*i] == ' ')
			env_value = ft_strdup(shell, "$");
		else if (base[*i] == '$')
			env_value = ft_getpid(shell);
		else if (base[*i] == '?')
			env_value = ft_itoa(shell, shell->last_exit_status);
	}
	return (env_value);
}


/*extract from base a substring and append it to new_str*/
char	*extract_substr_and_append(t_minishell *shell, char *base, int len,
		char *new_str)
{
	char	*suffix;

	suffix = ft_substr(shell, base, 0, len);
	new_str = append_suffix(shell, new_str, suffix);
	free(suffix);
	return (new_str);
}

/*skip over everything in single quotes and if you find an environment variable,
	overwrite it with its value*/
char	*expand_env_variables(t_minishell *shell, char *s)
{
	int		i;
	int		start;
	char	*new_str;
	char	*env_value;

	i = 0;
	start = 0;
	new_str = NULL;
	while (s[i])
	{
		if (s[i] == '\'')
			i = skip_between_quotes(s, i, '\'');
		else if (s[i] == '$')
		{
			new_str = extract_substr_and_append(shell, s + start, i - start,
					new_str);
			env_value = get_env_value(shell, s, &start, &i);
			new_str = append_suffix(shell, new_str, env_value);
			free(env_value);
		}
		i++;
	}
	if (i != start)
		new_str = extract_substr_and_append(shell, s + start, i - start,
				new_str);
	return (new_str);
}

char	*append_heredoc(t_minishell *shell, char *base, char quote_type)
{
	char	quote[2];
	char	*new_str;

	quote[0] = quote_type;
	quote[1] = '\0';
	new_str = NULL;
	new_str = append_suffix(shell, base, quote);
	new_str = append_suffix(shell, new_str, "<<");
	new_str = append_suffix(shell, new_str, quote);
	return (new_str);
}

/*if there was an odd number of quotes,
	this will add a heredoc at the end with first quote as EOF marker*/
char	*append_heredoc_on_missing_quote(t_minishell *shell, char *base)
{
	int		i;
	char	quote_type;

	i = 0;
	while (base[i])
	{
		if (base[i] == '"' || base[i] == '\'')
		{
			quote_type = base[i];
			i = skip_between_quotes(base, i, quote_type);
			if (!base[i])
				return (append_heredoc(shell, base, quote_type));
		}
		i++;
	}
	return (base);
}
