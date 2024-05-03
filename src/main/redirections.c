#include "../../inc/minishell.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void	redirect_output(char *output_file, int delimiter)
{
	int	output_fd;

	if (delimiter == OUTPUT)
		output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
	 	output_fd = open(output_file, O_WRONLY | O_APPEND);
	if (output_fd > 0)
	{
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
	else
	{
		print_errno(output_file);
		exit(EXIT_FAILURE);
	}
}




void	redirect_input(char *input_file)
{
	int	input_fd;

	input_fd = open(input_file, O_RDONLY);
	if (input_fd > 0)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	else if (input_fd < 0)
	{
		print_errno(input_file);
		exit(EXIT_FAILURE);
	}
}



/*
regarding input & heredoc: only if it is the most right delimiter, is _stdin will be true, and thus STDIN for the command should be redirected
*/
void	handle_redirections(t_minishell *shell, t_command_list *list)
{
	char *command;

	command = NULL;
	while (list)
	{
		if (list->delimiter == COMMAND)
			command = list->token;
		else if (list->delimiter == INPUT)
			redirect_input(list->token);
		else if (list->delimiter == OUTPUT)
		{
			redirect_output(list->token, OUTPUT);
		}
		else if (list->delimiter == APPEND)
			redirect_output(list->token, APPEND);
		else if (list->delimiter == HEREDOC)
			redirect_input(shell->input_buffer);

		list = list->next;
	}
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
void	append_to_command(t_command_list **head, char *token)
{
	t_command_list	*current;

	current = *head;
	while (current)
	{
		if (current->delimiter == COMMAND)
			current->token = ft_strjoin(current->token,
					token);
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


void	list_add(t_command_list **head, char *token, int type)
{
	t_command_list	*new;
	t_command_list	*current;

	new = malloc(sizeof(t_command_list));
	new->token = token;
	new->delimiter = type;
	new->next = NULL;
	if (!*head)
	{
		*head = new;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new;
}


void	extract_token(char *command, int start, int len,
		int preceding_delimiter, t_command_list **list)
{
	char *token;
	char *command_remainder;
	int end_index;

	token = ft_substr(command, start, len);
	token = ft_strtrim(token, " ");
	if (preceding_delimiter != COMMAND)
	{
		command_remainder = strchr(token, ' ');
		if (command_remainder)
		{
			end_index = command_remainder - token;
			command_remainder = strdup(command_remainder);
			token[end_index] = '\0';
			append_to_command(list, command_remainder);
		}
	}
	list_add(list, token, preceding_delimiter);
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
			extract_token(command, start, len, preceding_delimiter,
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
		extract_token(command, start, len, preceding_delimiter, &list);
	}
	return (list);
}
