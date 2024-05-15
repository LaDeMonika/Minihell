#include "../../inc/minishell.h"
#include <stdbool.h>

int	find_redirector(t_minishell *shell, char *command, int i)
{
	(void)shell;
	if (command[i] == '|')
		return (INVALID_PIPE);
	else if (command[i] == '<')
	{
		if (command[i + 1] == '<')
		{
			return (HEREDOC);
		}
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
void	append_to_command(t_minishell *shell, t_token_list **head, char *token)
{
	t_token_list	*current;

	current = *head;
	while (current)
	{
		if (current->delimiter == COMMAND)
			current->token = ft_strjoin(shell, current->token, token);
		current = current->next;
	}
}

void	list_add(t_token_list **head, char *token, int type)
{
	t_token_list	*new;
	t_token_list	*current;

	new = malloc(sizeof(t_token_list));
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
bool	has_unclosed_quote(char *s)
{
	int		i;
	char	metaquote;

	i = 0;
	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
		{
			metaquote = s[i];
			i++;
			while (s[i] && s[i] != metaquote)
				i++;
			if (!s[i])
				return (true);
		}
		i++;
	}
	return (false);
}

/*
extracts whatever everything between redirection symbols,
	start of string or end of string
if token is not a command argument, also remove outer quotes
*/
void	extract_token(t_minishell *shell, char *command, int start, int len,
		int pre_redirector, t_token_list **list)
{
	char	*token;
	char	*command_arg;
	int		end_index;
	int		i;
	char	outer_quote;

	token = ft_substr(shell, command, start, len);
	token = ft_strtrim(shell, token, " ");
	if (pre_redirector != COMMAND)
	{
		i = 0;
		outer_quote = '\0';
		// skips first pair of quotes
		while (token[i])
		{
			if (token[i] == '"' || token[i] == '\'')
			{
				outer_quote = token[i];
				i++;
				while (token[i] && token[i] != outer_quote)
					i++;
				break ;
			}
			i++;
		}
		if (!outer_quote)
			i = 0;
		command_arg = token + i;
		command_arg = strchr(command_arg, ' ');
		if (command_arg)
		{
			end_index = command_arg - token;
			command_arg = strdup(command_arg);
			token[end_index] = '\0';
			append_to_command(shell, list, command_arg);
		}
		if (!has_unclosed_quote(token))
			token = remove_metaquotes(shell, token);
	}
	list_add(list, token, pre_redirector);
}

void	adjust_indexes(int *i, int *start, int *len, int redirector)
{
	if (redirector == HEREDOC || redirector == APPEND)
		*i += 1;
	*start = *i + 1;
	*len = 0;
}

/*
find delimiter and add those parts to a linked list with info of delimiter kind
ignore redirect symbols if they are in quotes
if no delimiter is found, it will only add the command to list
*/
void	tokenize(t_minishell *shell, char *command, t_token_list **list)
{
	int		i;
	int		start;
	int		len;
	//int		temp;
	bool	redirector_processed;

	i = 0;
	start = 0;
	len = 0;
	shell->pre_redirector = COMMAND;
	redirector_processed = true;
	while (command[i])
	{
		shell->post_redirector = find_redirector(shell, command, i);
		if (shell->post_redirector > -1)
		{
			extract_token(shell, command, start, len, shell->pre_redirector,
				list);
			adjust_indexes(&i, &start, &len, shell->post_redirector);
			shell->pre_redirector = shell->post_redirector;
			redirector_processed = false;
		}
		else if (command[i] == '"' || command[i] == '\'')
		{
			len += skip_between_quotes(command, i, command[i]) + 1;
			i += skip_between_quotes(command, i, command[i]);
		}
		else
			len++;
		i++;
	}
	if (i != start || !redirector_processed)
		extract_token(shell, command, start, len, shell->pre_redirector, list);
}
