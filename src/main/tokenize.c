#include "../../inc/minishell.h"
#include <stdbool.h>

int	find_delimiter(char *command, int i)
{
	if (command[i] == '|')
		return (INVALID_PIPE);
	else if (command[i] == '<')
	{
		if (command[i + 1] == '<')
			return (HEREDOC);
		return (INPUT);
	}
	else if (command[i] == '>')
	{
		if (command[i + 1] == '>')
			return (APPEND);
		return (OUTPUT);
	}
	return (-1);
}
void	append_to_command(t_minishell *shell, t_token_list **head,
		char *command_arg, char *token)
{
	t_token_list	*current;
	int				end_index;

	end_index = command_arg - token;
	command_arg = strdup(command_arg);
	token[end_index] = '\0';
	current = *head;
	while (current)
	{
		if (current->delimiter == COMMAND)
			current->token = append_suffix(shell, current->token, command_arg);
		current = current->next;
	}
}

void	list_add(t_minishell *shell, t_token_list **head, char *token)
{
	t_token_list	*new;
	t_token_list	*current;

	new = try_malloc(shell, sizeof(t_token_list));
	new->token = token;
	new->delimiter = shell->pre_delimiter;
	new->next = NULL;
	if (!*head)
		*head = new;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new;
	}
}

/*
extracts whatever everything between delimiterion symbols,
	start of string or end of string
if token is not a command argument, also remove outer quotes
*/
void	extract_and_add_tokens(t_minishell *shell, int index, int start,
		int len)
{
	char	*token;
	char	*temp;
	char	*command_arg;

	token = ft_substr(shell, shell->input_array[index], start, len);
	temp = token;
	token = ft_strtrim(shell, token, " ");
	free_and_reset_ptr((void **)&temp);
	if (shell->pre_delimiter != COMMAND)
	{
		temp = token + skip_first_metaquote_pair(token);
		command_arg = strchr(temp, ' ');
		if (command_arg)
			append_to_command(shell, &shell->list[index], command_arg, token);
		if (has_even_metaquotes(token))
			token = remove_metaquotes(shell, token);
	}
	list_add(shell, &shell->list[index], token);
}

/*
find delimiter and add those parts to a linked list with info of delimiter kind
ignore delimiter symbols if they are in quotes
if no delimiter is found, it will only add the command to list
*/
void	tokenize(t_minishell *shell, char *command,
		int index)
{
	int		i;
	int		start;

	i = 0;
	start = 0;
	shell->pre_delimiter = COMMAND;
	while (command[i])
	{
		shell->post_delimiter = find_delimiter(command, i);
		if (shell->post_delimiter > -1)
		{
			extract_and_add_tokens(shell, index, start, i - start);
			if (shell->post_delimiter == HEREDOC
				|| shell->post_delimiter == APPEND)
				i++;
			start = i + 1;
			shell->pre_delimiter = shell->post_delimiter;
		}
		else if (command[i] == '"' || command[i] == '\'')
			i = skip_between_metaquotes(command, i, command[i]);
		i++;
	}
	if (i != start || shell->post_delimiter == shell->pre_delimiter)
		extract_and_add_tokens(shell, index, start, i - start);
}
