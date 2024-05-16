#include "../../inc/minishell.h"

void	handle_input(t_minishell *shell)
{
	int	i;

	if (strncmp(shell->usr_input, "exit", 5) == 0 || strncmp(shell->usr_input,
			"exit ", 5) == 0)
		error_free_all(shell, NO_ERROR, NULL, NULL);
	shell->usr_input = append_heredoc_on_missing_quote(shell, shell->usr_input);
	shell->usr_input = expand_env_variables(shell, shell->usr_input);
	shell->input_array = split_while_skipping_quotes(shell, shell->usr_input,
			'|');
	while (shell->input_array[shell->pipes_total + 1])
		shell->pipes_total++;
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
	if (shell->parsing_exit_status == 0)
		handle_pipes(shell, STDIN_FILENO);
	else
		shell->last_exit_status = shell->parsing_exit_status;
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	*shell;

	(void)argv;
	shell = NULL;
	shell = malloc(sizeof(t_minishell));
	if (!shell)
		return (perror("malloc"), 1);
	init_shell_struct(shell, envp);
	if (argc > 1)
		return (error_free_all(shell, ERR_TOO_MANY_ARGS, NULL, NULL), 1);
	while (1)
	{
		init_input_iteration(shell);
		set_signals(shell, PARENT_WITHOUT_CHILD);
		build_prompt(shell);
		shell->usr_input = readline(shell->prompt);
		if (!shell->usr_input)
			return (free_all(shell), 0);
		if (ft_strncmp(ft_strtrim(shell, shell->usr_input, " "), "\0", 1) != 0)
		{
			add_history(shell->usr_input);
			handle_input(shell);
		}
	}
	free_all(shell);
}
