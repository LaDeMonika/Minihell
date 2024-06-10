#include "../../inc/minishell.h"

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

	end_index = command_arg - *token;
	command_arg = ft_strdup(shell, command_arg);
	new_token = ft_substr(shell, *token, 0, end_index);
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

void	extract_and_add_tokens(t_minishell *shell, int index, int start,
		int len)
{
	char	*token;
	char	*temp;
	char	*command_arg;

	token = ft_substr(shell, shell->input_array[index], start, len);
	temp = token;
	token = ft_strtrim(shell, token, " ");
	if (shell->pre_delimiter != HEREDOC)
		token = expand_env_variables(shell, token);
	if (shell->pre_delimiter != COMMAND)
	{
		temp = token + skip_first_metaquote_pair(token);
		command_arg = strchr(temp, ' ');
		if (command_arg)
			append_to_command(shell, &shell->list[index], command_arg, &token);
		/* printf("checking command arg token: %s delimiter %d address of token: %p\n", token, shell->pre_delimiter,(void *)token); */
		/* char *old_token = token;*/
		if (shell->pre_delimiter != HEREDOC && has_even_metaquotes(token))
			token = remove_metaquotes(shell, token);
	}
	list_add(shell, &shell->list[index], token);
}

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
