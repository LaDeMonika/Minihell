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
void	append_to_command(t_command_list **head, char *token)
{
	t_command_list	*current;

	current = *head;
	while (current)
	{
		if (current->delimiter == COMMAND)
			current->token = ft_strjoin(current->token, token);
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
	/* printf("token with type %d and argument %s added\n", new->delimiter,
		new->token);*/
}
bool	has_unclosed_quote(char *s)
{
	int	i;
	char	quote_type;

	i = 0;
	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
		{
			quote_type = s[i];
			i++;
			while (s[i] && s[i] != quote_type)
				i++;
			if (!s[i])
				return (true);
		}
		i++;
	}
	return (false);
}

/*
extracts whatever everything between redirection symbols, start of string or end of string
if token is not a command argument, also remove outer quotes
*/
void	extract_token(char *command, int start, int len, int pre_redirector,
		t_command_list **list)
{
	char	*token;
	char	*command_arg;
	int		end_index;
	int	i;
	char	outer_quote;

	token = ft_substr(command, start, len);
	token = ft_strtrim(token, " ");
	if (pre_redirector != COMMAND)
	{
		i = 0;
		outer_quote = '\0';
		//skips first pair of quotes
		while (token[i])
		{
			if (token[i] == '"' || token[i] == '\'')
			{
				outer_quote = token[i];
				i++;
				while (token[i] && token[i] != outer_quote)
					i++;
				break;
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
			append_to_command(list, command_arg);
		}
		if (!has_unclosed_quote(token))
			token = remove_outer_quotes(token);
	}
	list_add(list, token, pre_redirector);
}

void	adjust_indexes(int *i, int *start, int *len, int redirector)
{
	if (redirector == HEREDOC || redirector == APPEND)
		*i += 2;
	else
		*i += 1;
	*start = *i;
	*len = 0;
}

/*
find delimiter and add those parts to a linked list with info of delimiter kind
if no delimiter is found, it will only add the command to list
*/
void	tokenize(t_minishell *shell, char *command, t_command_list **list)
{
	int		i;
	int		start;
	int		len;
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
			extract_token(command, start, len, shell->pre_redirector, list);
			adjust_indexes(&i, &start, &len, shell->post_redirector);
			shell->pre_redirector = shell->post_redirector;
			redirector_processed = false;
		}
		else if (command[i] == '"' || command[i] == '\'')
			skip_quotes(command, command[i], &i, &len);
		else
		{
			i++;
			len++;
		}
	}
	if (i != start || !redirector_processed)
		extract_token(command, start, len, shell->pre_redirector, list);
}
