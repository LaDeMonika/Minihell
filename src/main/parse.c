#include "../../inc/minishell.h"
#include <stdlib.h>
#include <unistd.h>

void	heredoc_EOF(t_minishell *shell, char *eof)
{
	char	*str_line_count;

	write(STDERR_FILENO, "bash: warning: here-document at line ", 37);
	str_line_count = ft_itoa(shell, shell->line_count);
	write(STDERR_FILENO, str_line_count, ft_strlen(str_line_count));
	write(STDERR_FILENO, " delimited by end-of-file (wanted `", 35);
	write(STDERR_FILENO, eof, ft_strlen(eof));
	write(STDERR_FILENO, "')\n", 3);
	free_and_reset_ptr((void **)&str_line_count);
}
/*writes input to input_file and sends for each input a newline character to pipe*/
void	write_to_file(t_minishell *shell, char *eof, char *input_file,
		int pipe_fd[2])
{
	int		file_fd;
	char	*input;

	close(pipe_fd[0]);
	set_signals(shell, HEREDOC_CHILD);
	file_fd = open(input_file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	input = readline("> ");
	while (input && ft_strncmp(input, eof, ft_strlen(eof) + 1) != 0)
	{
		write(pipe_fd[1], "\n", 1);
		write(file_fd, input, ft_strlen(input));
		write(file_fd, "\n", 1);
		input = readline("> ");
	}
	if (input)
		write(pipe_fd[1], "\n", 1);
	if (!input)
		heredoc_EOF(shell, eof);
	close(pipe_fd[1]);
	exit(EXIT_SUCCESS);
}

/*heredoc will read input in a child. local line count will increase by 1 for each line and added to global line count in the parent*/
void	heredoc(t_minishell *shell, char *eof, char *input_file)
{
	int		pid;
	int		pipe_fd[2];
	char	read_buffer[1];
	int		bytes_read;

	pipe(pipe_fd);
	pid = fork();
	if (pid == 0)
		write_to_file(shell, eof, input_file, pipe_fd);
	else
	{
		close(pipe_fd[1]);
		set_signals(shell, PARENT_WITH_CHILD);
		waitpid(pid, &shell->status, 0);
		bytes_read = read(pipe_fd[0], read_buffer, 1);
		while (bytes_read > 0)
		{
			shell->line_count++;
			bytes_read = read(pipe_fd[0], read_buffer, 1);
		}
		set_child_exit_status(shell, &shell->parsing_exit_status, 0);
		set_signals(shell, PARENT_WITHOUT_CHILD);
		close(pipe_fd[0]);
	}
}

void	error_parsing_input(t_minishell *shell, t_token_list *this, t_token_list *next)
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
	write(STDERR_FILENO, shell->unexpected_token, ft_strlen(shell->unexpected_token));
	write(STDERR_FILENO, "\n", 1);
	shell->parsing_exit_status = 2;
}
/*creates a list for each piped token and takes input for each heredoc
if the redirection syntax is wrong, it will print an error*/
void	parse_input(t_minishell *shell)
{
	int				i;
	char	*index;
	t_token_list	*list;

	shell->parsing_exit_status = 0;
	i = 0;
	while (shell->input_array[i])
	{
		index = NULL;
		index = ft_itoa(shell, i);

		shell->input_file = ft_strjoin(shell, index, "_input.txt");

		free_and_reset_ptr((void **)&index);

		list = shell->list[i];
		while (list)
		{

			if ((!list->token || !(*list->token)) && list->delimiter != COMMAND)
			{
				error_parsing_input(shell, list, shell->list[i + 1]);
				return ;
			}
			else if (list->delimiter == HEREDOC)
				heredoc(shell, list->token, shell->input_file);
			list = list->next;
		}
		i++;
	}
}
