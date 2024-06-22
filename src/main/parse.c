#include "../../inc/minishell.h"

void	error_parsing_input(t_minishell *shell, t_token_list *this,
		t_token_list *next)
{
	if (this->delimiter == INVALID_PIPE)
		shell->unexpected_token = "`|'";
	else if (this->next)
	{
		if (this->next->delimiter == INPUT)
			shell->unexpected_token = "`<'";
		else if (this->next->delimiter == HEREDOC)
			shell->unexpected_token = "`<<'";
		else if (this->next->delimiter == OUTPUT)
			shell->unexpected_token = "`>'";
		else if (this->next->delimiter == APPEND)
			shell->unexpected_token = "`>>'";
		else if (this->next->delimiter == INVALID_PIPE)
			shell->unexpected_token = "`|'";
	}
	else if (next)
		shell->unexpected_token = "`|'";
	else
		shell->unexpected_token = "`newline'";
	write(STDERR_FILENO, "bash: syntax error near unexpected token ", 41);
	write(STDERR_FILENO, shell->unexpected_token,
		ft_strlen(shell->unexpected_token));
	write(STDERR_FILENO, "\n", 1);
}

int	parse_token(t_minishell *shell, t_token_list *list)
{
	int	i;

	shell->parsing_exit_status = 0;
	i = 0;
	while (shell->input_array[i])
	{
		list = shell->list[i];
		while (list)
		{
			if ((!list->token || !(*list->token)) && list->delimiter != COMMAND)
			{
				error_parsing_input(shell, list, shell->list[i + 1]);
				shell->parsing_exit_status = 2;
				return (i);
			}
			list = list->next;
		}
		i++;
	}
	return (i);
}

void	handle_heredoc(t_minishell *shell, t_token_list *list,
		int error_at_index)
{
	int		j;
	char	*index;

	j = 0;
	while (shell->input_array[j])
	{
		list = shell->list[j];
		while (list)
		{
			if (list->delimiter == HEREDOC && j < error_at_index)
			{
				index = ft_itoa(shell, j);
				shell->input_file = append(shell, index, "_input.txt",
						BASE);
				heredoc(shell, &list->token, shell->input_file);
				if (shell->parsing_exit_status != 0
					&& unlink(shell->input_file) == -1)
					error_free_all(shell, ERR_UNLINK, shell->input_file, NULL);
				free_and_reset_ptr((void **)&shell->input_file);
			}
			list = list->next;
		}
		j++;
	}
}

void	parse_input(t_minishell *shell)
{
	int				i;
	int				error_at_index;
	t_token_list	*list;

	shell->usr_input = append_heredoc_on_missing_quote(shell, shell->usr_input);
	shell->temp_str = NULL;
	split_while_skipping_quotes(shell, shell->usr_input, '|');
	while (shell->input_array[shell->pipes_total + 1])
		shell->pipes_total++;
	shell->pid = try_malloc(shell, sizeof(int) * (shell->pipes_total + 1));
	shell->list = try_malloc(shell, sizeof(t_token_list *) * (shell->pipes_total
				+ 2));
	shell->list[shell->pipes_total + 1] = NULL;
	i = 0;
	while (shell->input_array[i])
	{
		shell->list[i] = NULL;
		tokenize(shell, shell->input_array[i], i);
		i++;
	}
	list = NULL;
	error_at_index = parse_token(shell, list);
	handle_heredoc(shell, list, error_at_index);
}
