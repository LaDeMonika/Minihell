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
		if (command[i + 1] == '|')
			return (FORCE_WRITE);
		return (OUTPUT);
	}
	return (-1);
}
void	append_to_command(t_minishell *shell, t_token_list **head,
		char *command_arg, char **token)
{
	t_token_list	*current;
	int				end_index;
	char	*new_token;

	/* printf("about to append to command\n"); */
	end_index = command_arg - *token;
	command_arg = ft_strdup(shell, command_arg);
	new_token = ft_substr(shell, *token, 0, end_index);
	/* printf("about to free token: %p\n", *token); */
	free_and_reset_ptr((void **)token);
	*token = new_token;
	current = *head;
	while (current)
	{
		if (current->delimiter == COMMAND)
			current->token = append_suffix(shell, current->token, command_arg);
		free_and_reset_ptr((void **)&command_arg);
		current = current->next;
	}
}

void	list_add(t_minishell *shell, t_token_list **head, char *token)
{
	t_token_list	*new;
	t_token_list	*current;

	new = try_malloc(shell, sizeof(t_token_list));
	/* printf("address of new: %p token: %s\n", new, token); */
	new->token = token;
	new->delimiter = shell->pre_delimiter;
	new->next = NULL;
	/* printf("new token: %s delimiter: %d next: %p\n", new->token, new->delimiter, new->next); */
	if (!*head)
		*head = new;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new;
	}
	/* printf("address of new token: %p\n", (void *)new->token); */
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
	//free_and_reset_ptr((void **)&temp);
	/* printf("before expansion token: %s delimiter: %d\n", token, shell->pre_delimiter); */
	if (shell->pre_delimiter != HEREDOC)
	{
		token = expand_env_variables(shell, token);
		/* printf("token: %s delimiter %d address of token: %p\n", token, shell->pre_delimiter,(void *)token); */
	}

	/* printf("after expansion token: %s delimiter: %d\n", token, shell->pre_delimiter); */
	if (shell->pre_delimiter != COMMAND)
	{
		/* printf("checking if command arg remainder\n"); */
		temp = token + skip_first_metaquote_pair(token);
		command_arg = strchr(temp, ' ');
		if (command_arg)
			append_to_command(shell, &shell->list[index], command_arg, &token);
		/* printf("checking command arg token: %s delimiter %d address of token: %p\n", token, shell->pre_delimiter,(void *)token); */
		/* char *old_token = token; */
		if (has_even_metaquotes(token))
			token = remove_metaquotes(shell, token);
		/* printf("address of old token: %p\n", (void *)old_token);
		printf("after checking quotes token: %s delimiter %d address of token: %p\n", token, shell->pre_delimiter,(void *)token); */
	}

	/* printf("after check token: %s delimiter %d address of token: %p\n", token, shell->pre_delimiter,(void *)token); */
	/* printf("list index: %d\n", index); */
	/* printf("address of token after removing metaquotes: %p\n", (void *)token); */
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
				|| shell->post_delimiter == APPEND || shell->post_delimiter == FORCE_WRITE)
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
