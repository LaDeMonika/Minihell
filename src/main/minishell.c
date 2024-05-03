#include "../../inc/minishell.h"
#include <unistd.h>


void	heredoc(t_minishell *shell, char *eof, int i)
{
	int input_fd;
	char *input;
	int	local_line_count;
	char	*file_name;
;
	local_line_count = 0;
	file_name = ft_strjoin(ft_itoa(i), "_input.txt");
	input_fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	dup2(shell->stdin_copy, STDIN_FILENO);
	input = readline("> ");
	while (input && ft_strncmp(input, eof, ft_strlen(eof) + 1) != 0)
	{
		local_line_count++;
		write(input_fd, input, ft_strlen(input));
		write(input_fd, "\n", 1);
		input = readline("> ");
	}
	if (input)
		local_line_count++;
	if (!input)
	{
		/* dup2(shell->stderr_copy, STDERR_FILENO); */
		write(2, "bash: warning: here-document at line ", 37);
		write(2, shell->str_line_count, ft_strlen(shell->str_line_count));
		write(2, " delimited by end-of-file (wanted `", 35);
		write(2, eof, ft_strlen(eof));
		write(2, "')\n", 3);
	}
	add_to_line_count(shell, local_line_count);
}

void	process_heredoc(t_minishell *shell, t_command_list *list, int i)
{
	while (list)
	{
		if (list->delimiter == HEREDOC)
			heredoc(shell, list->command_part, i);
		list = list->next;
	}
}

void	process_heredocs(t_minishell *shell)
{
	int	i;

	shell->list = malloc(sizeof(t_command_list*) * (shell->pipes_total + 1));
	i = 0;
	while (shell->input_array[i])
	{
		shell->list[i] = handle_delimiters(shell, shell->input_array[i]);
		process_heredoc(shell, shell->list[i], i);
		i++;
	}

}

void	handle_input(t_minishell *shell)
{
	if (strncmp(shell->usr_input, "exit", 5) == 0 || strncmp(shell->usr_input,
			"exit ", 5) == 0)
		exit(EXIT_SUCCESS);
	shell->input_array = ft_split_ignore_quotes(shell, shell->usr_input, '|');
	while (shell->input_array[shell->pipes_total + 1])
		shell->pipes_total++;
	process_heredocs(shell);
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
