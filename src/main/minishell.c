#include "../../inc/minishell.h"

void	handle_input(t_minishell *shell)
{
	parse_input(shell);
	if (shell->pipes_total == 0)
	{
		split_while_skipping_quotes(shell, shell->list[0]->token, ' ');
		is_builtin(shell, shell->command_array[0]);
		if (shell->builtin == B_CD || shell->builtin == B_EXIT || shell->builtin == B_EXPORT || shell->builtin == B_UNSET)
			shell->stay_in_parent = true;
	}
	if (shell->stay_in_parent)
	{
		handle_redirections(shell, shell->list[0], STDIN_FILENO, 0);
		update_last_arg(shell);
	}
	else if (shell->parsing_exit_status == 0)
	{
		handle_pipes(shell, shell->input_array, shell->pipes_total,
			STDIN_FILENO);
		update_last_arg(shell);
	}
	else
		shell->last_exit_status = shell->parsing_exit_status;
}

#include "../get_next_line/get_next_line.h"
char	*tester_readline(t_minishell *shell, char *prompt)
{
	char *line;

	if (isatty(fileno(stdin)))
		return (readline(prompt));
	else
	{
		line = get_next_line(fileno(stdin));
		return (ft_strtrim(shell, line, "\n"));
	}
}
// main for tester:
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
		// shell->usr_input = readline(shell->prompt); // uncomment before submission
		shell->usr_input = tester_readline(shell, shell->prompt);
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
