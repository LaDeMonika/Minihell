#include "../../inc/minishell.h"

char	*ft_getpid(t_minishell *shell)
{
	int		fd;
	char	buffer[1];
	char	*pid;
	int		i;

	pid = try_malloc(shell, sizeof(char) * 11);
	i = 0;
	fd = try_open(shell, "/proc/self/stat", READ);
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
		return (free_and_reset_ptr((void **)&pid), error_free_all(shell, ERR_OPEN, NULL, NULL), NULL);
	close(fd);
	return (free_and_reset_ptr((void **)&pid), error_free_all(shell, ERR_GETPID, NULL, NULL), NULL);
}

char	*ft_getenv(t_minishell *shell, char *key)
{
	int	i;
	char	*key_in_array;
	char	*value;

	i = 0;
	while (shell->envp[i])
	{
		key_in_array = ft_substr(shell, shell->envp[i], 0,  index_of_first_occurence(shell->envp[i], '='));
		if (ft_strcmp(key_in_array, key) == 0)
		{
			value = ft_substr(shell, shell->envp[i], index_of_first_occurence(shell->envp[i], '=') + 1, ft_strlen(strchr(shell->envp[i], '=') - 1));
			free_and_reset_ptr((void **)&key_in_array);
			return (value);
		}
		free_and_reset_ptr((void **)&key_in_array);
		i++;
	}
	return (NULL);
}

char	*get_env_value(t_minishell *shell, char *base, int *start, int *i,
		char *metaquote)
{
	char	*env_key;
	char	*env_value;

	env_key = NULL;
	env_value = NULL;
	(*i)++;
	if (base[*i - 1] == '~')
	{
		env_value = ft_strdup(shell, getenv("HOME"));
		if (base[*i])
			*start = *i + 1;
		else
			*start = *i;
	}
	else if (ft_isalpha(base[*i]) || (base[*i] == '_' && (base[*i + 1] == '_' || ft_isalnum(base[*i + 1]))))
	{

		*start = *i;
		while (ft_isalnum(base[*i]) || base[*i] == '_')
			(*i)++;
		env_key = ft_substr(shell, base, *start, *i - *start);
		env_value = ft_getenv(shell, env_key);
		free_and_reset_ptr((void **)&env_key);
		*start = *i;
		(*i)--;
	}
	else
	{

		if (!base[*i] || base[*i] == ' ' || base[*i] == *metaquote)
		{
			env_value = ft_strdup(shell, "$");
			(*i)--;
		}
		else if (base[*i] == '$')
			env_value = ft_getpid(shell);
		else if (base[*i] == '?')
			env_value = ft_itoa(shell, shell->last_exit_status);
		else if (base[*i] == '_')
			env_value = ft_getenv(shell, "_");
		else if (base[*i] == '"' || base[*i] == '\'')
			(*i)--;
		*start = *i + 1;
	}
	return (env_value);
}

char	*extract_substr_and_append(t_minishell *shell, char *base, int len,
		char *new_str)
{
	char	*suffix;

	suffix = ft_substr(shell, base, 0, len);
	new_str = append_suffix(shell, new_str, suffix);
	free_and_reset_ptr((void **)&suffix);
	return (new_str);
}

char	*expand_env_variables(t_minishell *shell, char *s)
{
	int		i;
	int		start;
	char	*new_str;
	char	*env_value;
	char	metaquote;

	i = 0;
	start = 0;
	new_str = NULL;
	metaquote = '\0';
	if (!s[i])
		new_str = ft_strdup(shell, s);
	while (s[i])
	{
		if (s[i] == '"' && !metaquote)
			metaquote = s[i];
		else if (s[i] == metaquote)
			metaquote = '\0';
		else if (s[i] == '\'' && !metaquote)
			i = skip_between_metaquotes(s, i, '\'');
		else if (s[i] == '$' || (s[i] == '~' && !metaquote && (is_space(s[i + 1]) || !s[i + 1])))
		{
			new_str = extract_substr_and_append(shell, s + start, i - start, new_str);
			env_value = get_env_value(shell, s, &start, &i, &metaquote);
			new_str = append_suffix(shell, new_str, env_value);
			free_and_reset_ptr((void **)&env_value);
		}
		if (s[i])
			i++;
	}
	if (i != start)
		new_str = extract_substr_and_append(shell, s + start, i - start,
				new_str);
	free_and_reset_ptr((void **)&s);
	return (new_str);
}

char	*append_heredoc(t_minishell *shell, char *base, char metaquote)
{
	char	quote[2];
	char	*new_str;

	quote[0] = metaquote;
	quote[1] = '\0';
	new_str = NULL;
	new_str = append_suffix(shell, base, quote);
	new_str = append_suffix(shell, new_str, "<<");
	new_str = append_suffix(shell, new_str, quote);
	return (new_str);
}

char	*append_heredoc_on_missing_quote(t_minishell *shell, char *base)
{
	int		i;
	char	metaquote;

	i = 0;
	while (base[i])
	{
		if (base[i] == '"' || base[i] == '\'')
		{
			metaquote = base[i];
			i = skip_between_metaquotes(base, i, metaquote);
			if (!base[i])
				return (append_heredoc(shell, base, metaquote));
		}
		i++;
	}
	return (base);
}
