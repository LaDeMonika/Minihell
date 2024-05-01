#include "../../inc/minishell.h"
#include <stdlib.h>
#include <unistd.h>

char	*remove_quotes(char *command)
{
	char *new_str;
	int	new_len;
	int	i;
	int	j;

	i = 0;
	new_len = 0;
	while (command[i])
	{
		if (command[i] != '"' && command[i] != '\'')
			new_len++;
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
	int	i;

	(void)shell;
	command_array = ft_split(command, ' ');
	i = 0;
	while (command_array[i])
	{
		command_array[i] = remove_quotes(command_array[i]);
		i++;
	}

	// builtins
	is_builtin = ft_is_builtin(shell, command_array);
	path = NULL;
	if (strncmp(command_array[0], "./", 2) != 0)
		path = find_command(command_array);
	else
		path = command_array[0];
	//dup2(shell->stderr_copy, STDERR_FILENO);
	printf("command %s before checking condition for execve\n", command_array[0]);
	if (is_builtin == 1 && !shell->error)
	{
		printf("command %s about to do execve\n", command_array[0]);
		execve(path, command_array, envp);
	}
	// TODO: also set exit status and custom message for builtins
	//write(2, "execve skipped\n", 16);

	/* if (shell->error)
	{
		write(2, "error before execve\n", 21);
		write(2, shell->error, ft_strlen(shell->error));
		write(2, "\n", 1);
	} */
	if (shell->error)
		exit(EXIT_FAILURE);
	dup2(shell->stderr_copy, STDERR_FILENO);
	close(shell->stderr_copy);
	custom_message = set_exit_status(shell, &exit_status);
	if (custom_message)
		custom_perror(ft_strjoin(command_array[0], ": "), custom_message);
	else
	{
		perror(ft_strjoin(command_array[0], ": "));
	}
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