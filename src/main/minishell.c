#include "../../inc/minishell.h"

void	postprocess(t_minishell *shell)
{
	int				i;
	t_token_list	*list;
	char	*space_index;
	char 	*last_command;

	i = 0;
	while (shell->input_array[i])
	{
		list = shell->list[i];
		while (list)
		{
			if (shell->pipes_total == 0 && list->delimiter == COMMAND && list->token)
			{
				if (list->token[0])
				{
					space_index = strchr(list->token, ' ');
					if (space_index)
					{
						while (space_index)
						{
							last_command = space_index + 1;
							space_index = strchr(last_command, ' ');
						}
					}
					else
						last_command = list->token;
					update_value(shell, "_", last_command, false);
				}
				else
					update_value(shell, "_", "", false);

			}
			list = list->next;
		}
		i++;
	}
}

void	handle_input(t_minishell *shell)
{
	int	i;

	shell->usr_input = append_heredoc_on_missing_quote(shell, shell->usr_input);
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
	parse_input(shell);
	if (shell->pipes_total == 0)
	{
		split_while_skipping_quotes(shell, shell->list[0]->token, ' ');
		is_builtin(shell, shell->command_array[0]);
	}
	if (shell->pipes_total == 0 && (shell->builtin == B_CD || shell->builtin == B_EXIT || shell->builtin == B_EXPORT || shell->builtin == B_UNSET))
	{
		shell->stay_in_parent = true;
		handle_redirections(shell, shell->list[0], STDIN_FILENO, 0);
	}
	else if (shell->parsing_exit_status == 0)
		handle_pipes_recursive(shell, shell->input_array, shell->pipes_total,
			STDIN_FILENO);
	else
		shell->last_exit_status = shell->parsing_exit_status;
	postprocess(shell);
}

/* int	main(int argc, char **argv, char **envp)
{
	t_minishell	*shell;

	(void)argv;
	shell = malloc(sizeof(t_minishell));
	if (!shell)
		return (perror("malloc"), EXIT_FAILURE);
	init_shell_struct(shell, envp);
	while (1)
	{
		init_input_iteration(shell);
		set_signals(shell, PARENT_WITHOUT_CHILD);
		build_prompt(shell);
		shell->usr_input = readline(shell->prompt);
		if (!shell->usr_input)
			return (free_all(shell));
		shell->usr_input = ft_strtrim(shell, shell->usr_input, " \n\f\r\t\v");
		if (ft_strcmp(shell->usr_input, "\0") != 0)
		{
			add_history(shell->usr_input);
			handle_input(shell);
		}
		free_iteration(shell);
	}
	return (free_all(shell));
} */
//main for tester:
#include "../get_next_line/get_next_line.h"
int	main(int argc, char **argv, char **envp)
{
	t_minishell	*shell;

	(void)argv;
	shell = malloc(sizeof(t_minishell));
	if (!shell)
		return (perror("malloc"), EXIT_FAILURE);
	init_shell_struct(shell, argc, envp);
	while (1)
	{
		init_input_iteration(shell);
		set_signals(shell, PARENT_WITHOUT_CHILD);
		build_prompt(shell);
		if (isatty(fileno(stdin)))
			shell->usr_input = readline(shell->prompt);
		else
		{
			char *line;
			line = get_next_line(fileno(stdin));
			shell->usr_input = ft_strtrim(shell, line, "\n");
		}
		if (!shell->usr_input)
			return (free_all(shell));
		shell->usr_input = ft_strtrim(shell, shell->usr_input, " \n\f\r\t\v");
		if (ft_strcmp(shell->usr_input, "\0") != 0)
		{
			add_history(shell->usr_input);
			handle_input(shell);
		}
		free_iteration(shell);
	}
	return (free_all(shell));
}
