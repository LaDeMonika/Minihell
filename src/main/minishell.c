#include "../../inc/minishell.h"

char	*find_command(char **input_array)
{
	char	*path;
	char	**path_array;
	int		i;
	char	*command_path;
	int		fd;

	path = getenv("PATH");
	if (!path)
		return (ft_error_msg(ERR_PATH_NOT_FOUND), NULL);
	path_array = ft_split(path, ':');
	i = 0;
	while (path_array[i])
	{
		command_path = ft_strjoin(path_array[i], "/");
		command_path = ft_strjoin(command_path, input_array[0]);
		fd = access(command_path, F_OK & X_OK);
		if (fd == 0)
		{
			return (command_path);
		}
		i++;
	}
	return (NULL);
}

void	custom_perror(char *prefix, char *custom_message)
{
	write(2, prefix, ft_strlen(prefix));
	write(2, custom_message, ft_strlen(custom_message));
	write(2, "\n", 1);
}

char	*set_exit_status(int *exit_status)
{
	*exit_status = EXIT_FAILURE;
	if (errno == EACCES)
		*exit_status = 126;
	else if (errno == EFAULT || errno == ENOENT)
	{
		*exit_status = 127;
		if (errno == EFAULT)
			return ("command not found");
	}
	return (NULL);
}

void	list_add(t_command_list **head, char *command_part, int type)
{
	t_command_list	*new;
	t_command_list	*current;

	new = malloc(sizeof(t_command_list));
	new->command_part = command_part;
	new->delimiter = type;
	new->next = NULL;
	if (type == HEREDOC || type == INPUT)
		new->primary_input = true;
	if (!*head)
	{
		*head = new;
		return ;
	}
	current = *head;
	while (current->next)
	{
		if (new->delimiter == HEREDOC && (current->delimiter == HEREDOC
				|| current->delimiter == INPUT))
			current->primary_input = false;
		current = current->next;
	}
	if (new->delimiter == HEREDOC && (current->delimiter == HEREDOC
			|| current->delimiter == INPUT))
		current->primary_input = false;
	current->next = new;
}

void	append_to_command(t_command_list **head, char *command_part)
{
	t_command_list	*current;

	current = *head;
	while (current)
	{
		if (current->delimiter == COMMAND)
			current->command_part = ft_strjoin(current->command_part,
					command_part);
		current = current->next;
	}
}
/*ignore redirect symbols if they are in quotes*/
void	skip_quotes(char *command, char quote_marker, int *i, int *len)
{
	(*i)++;
	(*len)++;
	while (command[*i] && command[*i] != quote_marker)
	{
		(*i)++;
		(*len)++;
	}
	if (command[*i] && command[*i] == quote_marker)
	{
		(*i)++;
		(*len)++;
	}

}

/*
find delimiter and add those parts to a linked list with info of delimiter kind
if no delimiter is found, it will only add the command to list
*/
void	handle_delimiters(t_minishell *shell, char *command, char **envp)
{
	int				i;
	int				preceding_delimiter;
	int				succeeding_delimiter;
	t_command_list	*list;
	int				start;
	int				len;

	list = NULL;
	i = 0;
	start = 0;
	len = 0;
	preceding_delimiter = COMMAND;
	succeeding_delimiter = -1;
	while (command[i])
	{
		succeeding_delimiter = find_delimiter(command[i], command[i + 1]);
		if (succeeding_delimiter > -1)
		{
			extract_command_part(command, start, len, preceding_delimiter,
				&list);
			if (succeeding_delimiter == HEREDOC
				|| succeeding_delimiter == APPEND)
				i += 2;
			else
				i += 1;
			preceding_delimiter = succeeding_delimiter;
			start = i;
			len = 0;
		}
		else if (command[i] == '"')
			skip_quotes(command, '"', &i, &len);
		else if (command[i] == '\'')
			skip_quotes(command, '\'', &i, &len);
		else
		{
			i++;
			len++;
		}
	}
	if (i != start)
	{
		extract_command_part(command, start, len, preceding_delimiter, &list);
	}
	handle_redirections(shell, list, envp);
}

void	parent(t_minishell *shell, char **input_array, int pipes_left,
		int read_fd)
{
	shell->sa_sigint.sa_handler = SIG_IGN;
	sigaction(SIGINT, &shell->sa_sigint, NULL);
	close(shell->pipe_fd[1]);
	if (read_fd > 0)
		close(read_fd);
	if (pipes_left >= 1)
	{
		handle_pipes_recursive(shell, input_array + 1, pipes_left - 1,
			shell->pipe_fd[0]);
	}
	else
	{
		while ((waitpid(shell->pid[shell->pipes_total], &shell->status, 0)) > 0)
		{
			shell->pipes_total--;
		}
		set_last_exit_status(shell);
	}
	close(shell->pipe_fd[0]);
}

void	child(t_minishell *shell, char **input_array, int pipes_left,
		int read_fd)
{
	set_child_signals(shell);
	close(shell->pipe_fd[0]);
	if (read_fd > 0)
	{
		dup2(read_fd, STDIN_FILENO);
		close(read_fd);
	}
	if (pipes_left >= 1)
	{
		dup2(shell->pipe_fd[1], STDOUT_FILENO);
	}
	close(shell->pipe_fd[1]);
	handle_delimiters(shell, input_array[0], shell->envp);
}

void	handle_pipes_recursive(t_minishell *shell, char **input_array,
		int pipes_left, int read_fd)
{
	pipe(shell->pipe_fd);
	shell->pid[pipes_left] = fork();
	if (shell->pid[pipes_left] < 0)
	{
		perror("fork");
	}
	else if (shell->pid[pipes_left] > 0)
	{
		parent(shell, input_array, pipes_left, read_fd);
	}
	else if (shell->pid[pipes_left] == 0)
	{
		child(shell, input_array, pipes_left, read_fd);
	}
}

// no pipe: execute directly
// last child: redirect IN, but keep OUT same:
// first child - redirect OUT, but keep IN same:
// middle child - redirect IN and OUT
// if not stdin, close IN of last iteration
// if at least one pipe, fork
// if no pipe (anymore return)
// close all the read ends of pipes before returning from recursive call
void	handle_pipes(t_minishell *shell, int read_fd)
{
	shell->pid = malloc(sizeof(int) * (shell->pipes_total + 2));
	handle_pipes_recursive(shell, shell->input_array, shell->pipes_total,
		read_fd);
}

void	handle_input(t_minishell *shell)
{
	if (strncmp(shell->usr_input, "exit", 5) == 0 || strncmp(shell->usr_input,
			"exit ", 5) == 0)
		exit(EXIT_SUCCESS);
	shell->input_array = ft_split_ignore_quotes(shell, shell->usr_input, '|');
	while (shell->input_array[shell->pipes_total + 1])
		shell->pipes_total++;
	handle_pipes(shell, STDIN_FILENO);
}
void	read_line_count(t_minishell *shell)
{
	int line_count_fd;
	int bytes_read;

	//line_count = malloc(sizeof(char *) * 16);
	line_count_fd = open("line_count.txt", O_RDONLY);
	bytes_read = read(line_count_fd, shell->line_count, 16);
	close(line_count_fd);
}

void	add_to_line_count(char *old_line_count, int new_lines)
{
	char *new_line_count;
	int line_count_fd;
	int i_new_line_count;
	int	i_old_line_count;

	i_old_line_count = ft_atoi(old_line_count);
	i_new_line_count = i_old_line_count + new_lines;
	new_line_count = ft_itoa(i_new_line_count);
	line_count_fd = open("line_count.txt", O_WRONLY);
	write(line_count_fd, new_line_count, 16);
	close(line_count_fd);
}

void	init_line_count(t_minishell *shell)
{
	int line_count_fd;
	int bytes_written;

	shell->line_count = malloc(sizeof(char) * 16);
	line_count_fd = open("line_count.txt", O_WRONLY | O_CREAT | O_TRUNC, 0777);
	ft_bzero(shell->line_count, 16);
	shell->line_count[0] = '0';
	bytes_written = write(line_count_fd, shell->line_count, 16);
	close(line_count_fd);

}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	*shell;

	(void)argc;
	(void)argv;
	shell = malloc(sizeof(t_minishell));
	if (!shell)
		return (ft_error_msg(ERR_MALLOC), 1);
	init_shell_struct(shell, envp);
	init_line_count(shell);
	while (1)
	{
		set_signals_parent(shell);
		build_prompt(shell);
		shell->usr_input = readline(shell->prompt);
		if (ft_strncmp(shell->usr_input, "\0", 1) != 0)
		{
			add_to_line_count(shell->line_count, 1);
			add_history(shell->usr_input);
			handle_input(shell);
			shell->sa_sigint.sa_handler = sigint_handler;
			sigaction(SIGINT, &shell->sa_sigint, NULL);
			free(shell->usr_input);
		}
		free(shell->prompt);
		shell->prompt = NULL;
		//printf("line count: %d\n", line_count);
	}
	free(shell->line_count);
	free(shell);
}
