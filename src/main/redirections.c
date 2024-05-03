#include "../../inc/minishell.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void	redirect_output(char *output_file, int delimiter, bool *error)
{
	int	output_fd;

	if (delimiter == OUTPUT)
		output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
	 	output_fd = open(output_file, O_WRONLY | O_APPEND);
	if (output_fd > 0)
		dup2(output_fd, STDOUT_FILENO);
	else
	{
		if (!(*error))
		{
			append_strerror_to_log(output_file, errno);
			*error = true;
		}
	}

	close(output_fd);
}




void	redirect_input(char *input_file, bool is_stdin, bool *error)
{
	int	input_fd;

	/* printf("about to redirect?\n"); */
	input_fd = open(input_file, O_RDONLY);
	if (input_fd > 0 && is_stdin)
		dup2(input_fd, STDIN_FILENO);
	else if (input_fd < 0)
	{
		/* printf("opening file failed\n"); */
		if (!(*error))
		{
			*error = true;
			/* printf("saving errno to error variable\n"); */
			append_strerror_to_log(input_file, errno);
		}
	}
	close(input_fd);

}



/*
regarding input & heredoc: only if it is the most right delimiter, is _stdin will be true, and thus STDIN for the command should be redirected
*/
void	handle_redirections(t_minishell *shell, t_command_list *list, bool is_last_child, int i)
{
	char *command;
	bool	output_redirect;

	command = NULL;
	shell->error = NULL;
	output_redirect = false;
	char *file_name;
	/* redirect_errors(); */
	file_name = ft_strjoin(ft_itoa(i), "_input.txt");
	while (list)
	{
		if (list->delimiter == COMMAND)
			command = list->command_part;
		else if (list->delimiter == INPUT)
			redirect_input(list->command_part, list->is_stdin, &shell->error);
		else if (list->delimiter == OUTPUT)
		{
			redirect_output(list->command_part, OUTPUT, &shell->error);
			output_redirect = true;
		}
		else if (list->delimiter == APPEND)
			redirect_output(list->command_part, APPEND, &shell->error);
		else if (list->delimiter == HEREDOC)
		{
			/* printf("stdin copy fd: %d\n", shell->stdin_copy);
			printf("stderr copy fd: %d\n", shell->stderr_copy); */
			/* dup2(shell->stdin_copy, STDIN_FILENO); */
			/* if (!is_last_child)
				dup2(shell->stdout_copy, STDOUT_FILENO); */
			//heredoc(shell, list->command_part);
			if (list->is_stdin)
				redirect_input(file_name, list->is_stdin, &shell->error);
			/* if (!is_last_child)
				dup2(shell->pipe_fd[1], STDOUT_FILENO); */
		}
		list = list->next;
	}
	close(shell->stdin_copy);
	//close(shell->stderr_copy);
	close(shell->stdout_copy);
	if (!is_last_child)
		close(shell->pipe_fd[1]);
	else if (is_last_child && !output_redirect)
	 	redirect_stdout_to_log();
	execute_command(shell, command);
}
int	find_delimiter(t_minishell *shell, char c1, char c2)
{
	(void)shell;
	if (c1 == '<')
	{
		if (c2 == '<')
		{
			return HEREDOC;
		}
		return INPUT;
	}
	else if (c1 == '>')
	{
		if (c2 == '>')
			return APPEND;
		return OUTPUT;
	}
	return (-1);
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


void	list_add(t_command_list **head, char *command_part, int type)
{
	t_command_list	*new;
	t_command_list	*current;

	new = malloc(sizeof(t_command_list));
	new->command_part = command_part;
	new->delimiter = type;
	new->next = NULL;
	if (type == HEREDOC || type == INPUT)
		new->is_stdin = true;


	if (!*head)
	{
		*head = new;
		return ;
	}
	current = *head;
	while (current->next)
	{
		if (new->is_stdin && (current->delimiter == INPUT || current->delimiter == HEREDOC))
			current->is_stdin = false;
		current = current->next;
	}
	if (new->is_stdin && (current->delimiter == INPUT || current->delimiter == HEREDOC))
			current->is_stdin = false;
	current->next = new;
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

/*
find delimiter and add those parts to a linked list with info of delimiter kind
if no delimiter is found, it will only add the command to list
*/
t_command_list	*handle_delimiters(t_minishell *shell, char *command)
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
		succeeding_delimiter = find_delimiter(shell, command[i], command[i + 1]);
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
	return (list);
	//handle_redirections(shell, list, envp, is_last_child);
}
