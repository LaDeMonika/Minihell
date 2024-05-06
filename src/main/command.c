#include "../../inc/minishell.h"

char	*remove_outer_quotes(char *command)
{
	char	*new_str;
	int		new_len;
	int		i;
	int		j;
	char	quote_type;

	i = 0;
	new_len = 0;
	quote_type = '\0';
	while (command[i])
	{
		if (command[i] == '"' && command[i] != '\'')
		{
			new_len++;
		}
		else
		{
			if (quote_type)
			{
				if (command[i] != quote_type)
					new_len++;
			}
			else
				quote_type = command[i];
		}
		i++;
	}
	new_str = malloc(sizeof(char) * (new_len + 1));
	i = 0;
	j = 0;
	while (command[i])
	{
		if (command[i] != '"' && command[i] != '\'')
		{
			new_str[j] = command[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

void	execute_command(t_minishell *shell, char *command, char **envp)
{
	int		is_builtin;
	char	**command_array;
	char	*path;
	int		exit_status;
	char	*custom_message;
	int		i;

	// int		is_builtin;
	(void)shell;
	command_array = ft_split(command, ' ');
	i = 0;
	while (command_array[i])
	{
		command_array[i] = remove_outer_quotes(command_array[i]);
		i++;
	}
	// builtins
	is_builtin = ft_is_builtin(shell, command_array);
	path = NULL;
	if (strncmp(command_array[0], "./", 2) != 0)
		path = find_command(command_array);
	else
		path = command_array[0];
	if (is_builtin == 1)
	{
		execve(path, command_array, envp);
	}
	// TODO: also set exit status and custom message for builtins
	custom_message = set_exit_status(&exit_status);
	if (custom_message)
		custom_perror(ft_strjoin(command_array[0], ": "), custom_message);
	else
		perror(ft_strjoin(command_array[0], ": "));
	printf("command: %s errno: %d\n", command_array[0], errno);
	printf("command: %s exit status: %d\n", command_array[0], exit_status);
	exit(exit_status);
}

void	extract_command_part(char *command, int start, int len,
		int preceding_delimiter, t_command_list **list)
{
	char *command_part;
	char *command_remainder;
	int end_index;

	command_part = ft_substr(command, start, len);
	command_part = ft_strtrim(command_part, " ");
	if (preceding_delimiter != COMMAND)
	{
		command_remainder = strchr(command_part, ' ');
		if (command_remainder)
		{
			end_index = command_remainder - command_part;
			command_remainder = strdup(command_remainder);
			command_part[end_index] = '\0';
			append_to_command(list, command_remainder);
		}
	}
	list_add(list, command_part, preceding_delimiter);
}