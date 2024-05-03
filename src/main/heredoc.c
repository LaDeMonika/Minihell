#include "../../inc/minishell.h"
#include <stdlib.h>
#include <unistd.h>

void	heredoc_EOF(t_minishell *shell, char *eof)
{
	char *str_line_count;
	write(STDERR_FILENO, "bash: warning: here-document at line ", 37);
	str_line_count = ft_itoa(shell->line_count);
	/* write(STDERR_FILENO, shell->str_line_count,
		ft_strlen(shell->str_line_count)); */
	write(STDERR_FILENO, str_line_count,
		ft_strlen(str_line_count));
	write(STDERR_FILENO, " delimited by end-of-file (wanted `", 35);
	write(STDERR_FILENO, eof, ft_strlen(eof));
	write(STDERR_FILENO, "')\n", 3);
}
void	set_heredoc_exit_status(t_minishell *shell)
{
	if (WIFEXITED(shell->status))
	{
		shell->heredoc_exit_status = WEXITSTATUS(shell->status);
	}
	else if (WIFSIGNALED(shell->status))
	{
		write(2, "\n", 1);
		printf("before adding 128: %d\n", WTERMSIG(shell->status));
		shell->heredoc_exit_status = WTERMSIG(shell->status) + 128;
	}
	/* printf("exit status: %d\n", shell->last_exit_status); */
	if (WCOREDUMP(shell->status))
	{

		printf("some core was dumped\n");
		write(2, "^\\Quit (core dumped)\n", 21);
	}
}

void	heredoc(t_minishell *shell, char *eof, char *input_buffer)
{
	int		buffer_fd;
	char	*input;
	int		local_line_count;
	int		heredoc_pid;
	int	heredoc_fd[2];
	char	read_buffer[1];
	int	bytes_read;


	local_line_count = 0;
	buffer_fd = open(input_buffer, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	pipe(heredoc_fd);
	heredoc_pid = fork();
	if (heredoc_pid == 0)
	{
		close(heredoc_fd[0]);
		set_child_signals(shell);
		input = readline("> ");
		while (input && ft_strncmp(input, eof, ft_strlen(eof) + 1) != 0)
		{
			write(heredoc_fd[1], "\n", 1);
			//local_line_count++;
			write(buffer_fd, input, ft_strlen(input));
			write(buffer_fd, "\n", 1);
			input = readline("> ");
		}
		if (input)
			write(heredoc_fd[1], "\n", 1);
			//local_line_count++;
		if (!input)
			heredoc_EOF(shell, eof);
		close(heredoc_fd[1]);
		exit(EXIT_SUCCESS);
	}
	else
	{
		close(heredoc_fd[1]);
		shell->sa_sigint.sa_handler = SIG_IGN;
		sigaction(SIGINT, &shell->sa_sigint, NULL);
		waitpid(heredoc_pid, &shell->status, 0);
		bytes_read = read(heredoc_fd[0], read_buffer, 1);
		while (bytes_read > 0)
		{
			local_line_count++;
			bytes_read = read(heredoc_fd[0], read_buffer, 1);
		}
		shell->line_count += local_line_count;
		//add_to_line_count(shell, local_line_count);
		set_heredoc_exit_status(shell);
		set_signals_parent(shell);
		close(heredoc_fd[0]);
	}

}
/*i represent the index in the input array after splitting along the pipes*/
void	process_heredoc(t_minishell *shell, t_command_list *list,
		char *input_buffer)
{
	while (list)
	{
		if (list->delimiter== HEREDOC)
			heredoc(shell, list->token, input_buffer);
		list = list->next;
	}
}
/*creates a list for each piped token and takes input for each heredoc*/
void	process_heredocs(t_minishell *shell)
{
	int i;

	shell->heredoc_exit_status = 0;

	shell->list = malloc(sizeof(t_command_list *) * (shell->pipes_total + 1));
	i = 0;
	while (shell->input_array[i])
	{
		shell->input_buffer = ft_strjoin(ft_itoa(i), "_input.txt");
		shell->list[i] = handle_delimiters(shell, shell->input_array[i]);
		process_heredoc(shell, shell->list[i], shell->input_buffer);
		i++;
	}
}