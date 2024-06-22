#include "../../inc/minishell.h"

void	update_last_arg(t_minishell *shell)
{
	int				i;
	t_token_list	*list;

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
					update_value(shell, "_", last_word(list->token),
						false);
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
	close(fd);
	free_and_reset_ptr((void **)&shell->my_pid);
	return (ft_strdup(shell, "\0"));
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
	{
		shell->old_value = ft_getpid(shell);
		shell->my_pid = NULL;
	}
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
