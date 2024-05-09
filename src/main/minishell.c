#include "../../inc/minishell.h"
#include <unistd.h>



void	handle_input(t_minishell *shell)
{
	int	i;

	if (strncmp(shell->usr_input, "exit", 5) == 0 || strncmp(shell->usr_input,
			"exit ", 5) == 0)
		exit(EXIT_SUCCESS);
	shell->usr_input = add_heredoc_if_necessary(shell->usr_input);
	shell->usr_input = expand_env_variables(shell, shell->usr_input);
	shell->input_array = split_skip_quotes(shell, shell->usr_input, '|');
	shell->pipes_total = 0;
	while (shell->input_array[shell->pipes_total + 1])
		shell->pipes_total++;
	shell->list = malloc(sizeof(t_command_list *) * (shell->pipes_total + 2));
	shell->list[shell->pipes_total + 1] = NULL;
	i = 0;
	while (shell->input_array[i])
	{
		shell->list[i] = NULL;
		tokenize(shell, shell->input_array[i], &shell->list[i]);
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
	if (argc > 1)
		return (ft_error_msg(ERR_TOO_MANY_ARGS), 1);
	shell = malloc(sizeof(t_minishell));
	if (!shell)
		return (ft_error_msg(ERR_MALLOC), 1);
	init_shell_struct(shell, envp);
	while (1)
	{
		set_signals(shell, PARENT_NO_CHILD);
		build_prompt(shell);
		shell->usr_input = readline(shell->prompt);
		shell->line_count++;
		if (ft_strncmp(shell->usr_input, "\0", 1) != 0)
		{
			add_history(shell->usr_input);
			handle_input(shell);
			free(shell->usr_input);
		}
		free(shell->prompt);
		shell->prompt = NULL;
	}
	free(shell);
}
