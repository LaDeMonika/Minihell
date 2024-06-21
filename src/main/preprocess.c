#include "../../inc/minishell.h"

void	update_last_arg(t_minishell *shell)
{
	int				i;
	t_token_list	*list;
	char			*space_index;
	char			*last_command;

	i = 0;
	while (shell->input_array[i])
	{
		list = shell->list[i];
		while (list)
		{
			if (shell->pipes_total == 0 && list->delimiter == COMMAND
				&& list->token)
			{
				if (list->token[0])
				{
					space_index = strchr(list->token, ' ');
					if (space_index)
					{
						while (space_index)
						{
							last_command = space_index + 1;
							space_index = strchr(last_command, ' ');
						}
					}
					else
						last_command = list->token;
					update_value(shell, "_", last_command, false);
				}
				else
					update_value(shell, "_", "", false);
			}
			list = list->next;
		}
		i++;
	}
}

char	*ft_getpid(t_minishell *shell)
{
	int		fd;
	char	buffer[1];
	int		i;

	shell->my_pid = try_malloc(shell, sizeof(char) * 11);
	i = 0;
	fd = try_open(shell, "/proc/self/stat", READ);
	if (fd > 0)
	{
		while (read(fd, buffer, 1) > 0)
		{
			if (buffer[0] != ' ')
			{
				shell->my_pid[i] = buffer[0];
				i++;
			}
			else
			{
				shell->my_pid[i] = '\0';
				close(fd);
				return (shell->my_pid);
			}
		}
	}
	else
		return (free_and_reset_ptr((void **)&shell->my_pid),
			error_free_all(shell, ERR_OPEN, NULL, NULL), NULL);
	close(fd);
	return (free_and_reset_ptr((void **)&shell->my_pid), error_free_all(shell,
			ERR_GETPID, NULL, NULL), NULL);
}

char	*ft_getenv(t_minishell *shell, char *key)
{
	int		i;
	char	*key_in_array;
	char	*value;

	i = 0;
	while (shell->envp[i])
	{
		key_in_array = ft_substr(shell, shell->envp[i], 0,
				index_of_char(shell->envp[i], '='));
		if (ft_strcmp(key_in_array, key) == 0)
		{
			value = ft_substr(shell, shell->envp[i],
					index_of_char(shell->envp[i], '=') + 1,
					ft_strlen(strchr(shell->envp[i], '=') - 1));
			free_and_reset_ptr((void **)&key_in_array);
			return (value);
		}
		free_and_reset_ptr((void **)&key_in_array);
		i++;
	}
	return (NULL);
}

char	*get_env_for_special_symbol(t_minishell *shell, char *base, int *start,
		int *i)
{
	if (!base[*i] || base[*i] == ' ' || base[*i] == shell->metaquote)
	{
		shell->old_value = ft_strdup(shell, "$");
		(*i)--;
	}
	else if (base[*i] == '$')
		shell->old_value = ft_getpid(shell);
	else if (base[*i] == '?')
		shell->old_value = ft_itoa(shell, shell->last_exit_status);
	else if (base[*i] == '_')
		shell->old_value = ft_getenv(shell, "_");
	else if (base[*i] == '"' || base[*i] == '\'')
		(*i)--;
	*start = *i + 1;
	return (shell->old_value);
}

char	*get_env_value(t_minishell *shell, char *base, int *start, int *i)
{
	(*i)++;
	if (base[*i - 1] == '~')
	{
		shell->old_value = ft_getenv(shell, "HOME");
		if (base[*i])
			*start = *i + 1;
		else
			*start = *i;
	}
	else if (ft_isalpha(base[*i]) || (base[*i] == '_' && (base[*i + 1] == '_'
				|| ft_isalnum(base[*i + 1]))))
	{
		*start = *i;
		while (ft_isalnum(base[*i]) || base[*i] == '_')
			(*i)++;
		shell->old_key = ft_substr(shell, base, *start, *i - *start);
		shell->old_value = ft_getenv(shell, shell->old_key);
		free_and_reset_ptr((void **)&shell->old_key);
		*start = *i;
		(*i)--;
	}
	else
		shell->old_value = get_env_for_special_symbol(shell, base, start, i);
	return (shell->old_value);
}

char	*extract_substr_and_append(t_minishell *shell, char *base, int len)
{
	shell->suffix = ft_substr(shell, base, 0, len);
	shell->temp_str = append(shell, shell->temp_str, shell->suffix,
			FREE_BOTH);
	return (shell->temp_str);
}

bool	found_quote(t_minishell *shell, char *s, int *i)
{
	if (s[*i] == '"' && !(shell->metaquote))
			shell->metaquote = s[*i];
	else if (s[*i] == shell->metaquote)
			shell->metaquote = '\0';
	else if (s[*i] == '\'' && !(shell->metaquote))
			*i = skip_between_metaquotes(s, *i, '\'');
	else
		return (false);
	return (true);
}

char	*expand_env_variables(t_minishell *shell, char *s)
{
	int		i;
	int		start;

	i = 0;
	start = 0;
	shell->metaquote = '\0';
	if (!s[i])
		shell->temp_str = ft_strdup(shell, s);
	while (s[i])
	{
		if (!found_quote(shell, s, &i) && (s[i] == '$' || (s[i] == '~' && !shell->metaquote && (is_space(s[i + 1]) || !s[i + 1]))))
		{
			shell->temp_str = extract_substr_and_append(shell, s + start,
					i - start);
			shell->old_value = get_env_value(shell, s, &start, &i);
			shell->temp_str = append(shell, shell->temp_str,
					shell->old_value, FREE_BOTH);
			shell->old_value = NULL;
		}
		if (s[i])
			i++;
	}
	if (i != start)
		shell->temp_str = extract_substr_and_append(shell, s + start, i
				- start);
	free_and_reset_ptr((void **)&s);
	return (shell->temp_str);
}

char	*append_heredoc_on_missing_quote(t_minishell *shell, char *base)
{
	int		i;
	char	quote[2];

	i = 0;
	while (base[i])
	{
		if (base[i] == '"' || base[i] == '\'')
		{
			shell->metaquote = base[i];
			i = skip_between_metaquotes(base, i, shell->metaquote);
			if (!base[i])
			{
				quote[0] = shell->metaquote;
				quote[1] = '\0';
				shell->temp_str = append(shell, base, quote, FREE_BASE);
				shell->temp_str = append(shell, shell->temp_str, "<<",
						FREE_BASE);
				shell->temp_str = append(shell, shell->temp_str, quote,
						FREE_BASE);
				free_and_reset_ptr((void **)&base);
				return (shell->temp_str);
			}
		}
		i++;
	}
	return (base);
}
