#include "../../inc/minishell.h"

char	*prepare_last_arg(t_minishell *shell, char *str)
{
	int	i;
	int	end;
	char	quote;

	if (shell->last_arg)
		free_and_reset_ptr((void **)&shell->last_arg);
	i = ft_strlen(str) - 1;
	if (str[i] == '"' || str[i] == '\'')
	{
		quote = str[i];
		end = i;
		i--;
		while (i >= 0 && str[i] != quote)
			i--;
		shell->last_arg = ft_substr(shell, str, i + 1, end - i);
	}
	else
	{
		while (i >= 0 && !is_space(str[i]))
			i--;
		shell->last_arg = ft_substr(shell, str, i + 1, ft_strlen(str) - i);
	}
	return (ft_strtrim(shell, shell->last_arg, " "));
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
		shell->env_value = ft_strdup(shell, "$");
		(*i)--;
	}
	else if (base[*i] == '$')
	{
		shell->env_value = ft_getpid(shell);
		shell->my_pid = NULL;
	}
	else if (base[*i] == '?')
		shell->env_value = ft_itoa(shell, shell->last_exit_status);
	else if (base[*i] == '_')
		shell->env_value = ft_getenv(shell, "_");
	else if (base[*i] == '"' || base[*i] == '\'')
		(*i)--;
	*start = *i + 1;
	return (shell->env_value);
}

char	*get_env_value(t_minishell *shell, char *base, int *start, int *i)
{
	(*i)++;
	if (base[*i - 1] == '~')
	{
		shell->env_value = ft_getenv(shell, "HOME");
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
		shell->env_key = ft_substr(shell, base, *start, *i - *start);
		shell->env_value = ft_getenv(shell, shell->env_key);
		free_and_reset_ptr((void **)&shell->env_key);
		*start = *i;
		(*i)--;
	}
	else
		shell->env_value = get_env_for_special_symbol(shell, base, start, i);
	return (shell->env_value);
}
