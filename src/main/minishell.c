#include "../../inc/minishell.h"
#include <unistd.h>






void	handle_input(t_minishell *shell)
{
	if (strncmp(shell->usr_input, "exit", 5) == 0 || strncmp(shell->usr_input,
			"exit ", 5) == 0)
		exit(EXIT_SUCCESS);
	shell->input_array = ft_split_ignore_quotes(shell, shell->usr_input, '|');
	while (shell->input_array[shell->pipes_total + 1])
		shell->pipes_total++;
	handle_pipes(shell, STDIN_FILENO);
}



int	main(int argc, char **argv, char **envp)
{
	t_minishell	*shell;

	(void)argc;
	(void)argv;
	shell = malloc(sizeof(t_minishell));
	if (!shell)
		return (ft_error_msg(ERR_MALLOC), 1);
	init_shell_struct(shell, envp);
	init_line_count(shell);
	while (1)
	{
		shell->stdin_copy = dup(STDIN_FILENO);
		shell->stdout_copy = dup(STDOUT_FILENO);
		shell->error = false;
		set_signals_parent(shell);
		build_prompt(shell);
		shell->usr_input = readline(shell->prompt);
		read_line_count(shell);
		add_to_line_count(shell, 1);
		if (ft_strncmp(shell->usr_input, "\0", 1) != 0)
		{
			add_history(shell->usr_input);
			handle_input(shell);
			shell->sa_sigint.sa_handler = sigint_handler;
			sigaction(SIGINT, &shell->sa_sigint, NULL);
			free(shell->usr_input);
			//close(shell->stdin_copy);
		}
		free(shell->prompt);
		shell->prompt = NULL;
	}
	close(shell->stdin_copy);
	/* close(shell->stdout_copy);
	close(shell->stderr_copy); */
	free(shell);
}
