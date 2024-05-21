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

char	*extract_line(t_minishell *shell, char *input)
{
	char	*temp;
	char	*line;
	int	end_index;
	int	remain_len;

	temp = strchr(input, '\n');
	if (temp)
	{
		end_index = temp - input;
		/* printf("end index: %d\n", end_index); */
		remain_len = ft_strlen(input) - end_index;
		line = ft_substr(shell, input, 0, end_index);
		shell->heredoc_input = ft_strdup(shell, temp + 1);

	}
	else
	{
		line = ft_strdup(shell, input);
		shell->heredoc_input = NULL;
	}
	/* printf("remaining input: %s\n", shell->heredoc_input); */
	free(input);
	return (line);
}
/*writes input to input_file and sends for each input a newline character to pipe to increment line count*/
void	write_to_file(t_minishell *shell, char *eof, char *input_file,
		int pipe_fd[2])
{
	int		file_fd;
	char	*current_line;

	try_close(shell, pipe_fd[0]);
	set_signals(shell, HEREDOC_CHILD);
	file_fd = try_open(shell, WRITE_TRUNCATE, input_file);
	shell->heredoc_input = readline("> ");
	current_line = extract_line(shell, shell->heredoc_input);
	/* printf("last line: %s\n", last_line);
	printf("input: %s\n", input); */
	/* try_write(shell, file_fd, "\n", 1); */
	while (current_line && (ft_strncmp(current_line, eof, ft_strlen(eof) + 1) != 0))
	{
		current_line = expand_env_variables(shell, current_line);
		try_write(shell, pipe_fd[1], "\n", 1);
		try_write(shell, file_fd, current_line, ft_strlen(current_line));
		try_write(shell, file_fd, "\n", 1);
		/* printf("input: %s last line: %s\n", input, last_line); */
		if (!shell->heredoc_input)
			shell->heredoc_input = readline("> ");
		current_line = extract_line(shell, shell->heredoc_input);
	}
	if (current_line)
		try_write(shell, pipe_fd[1], "\n", 1);
	if (!current_line)
		heredoc_EOF(shell, eof);
	try_close(shell, pipe_fd[1]);
	exit(EXIT_SUCCESS);
}

/*heredoc will read input in a child. local line count will increase by 1 for each line and added to global line count in the parent*/
void	heredoc(t_minishell *shell, char *eof, char *input_file)
{
	int		pid;
	int		pipe_fd[2];
	char	*read_buffer;

	try_pipe(shell, pipe_fd);
	pid = try_fork(shell);
	if (pid == 0)
		write_to_file(shell, eof, input_file, pipe_fd);
	else
	{
		try_close(shell, pipe_fd[1]);
		set_signals(shell, PARENT_WITH_CHILD);
		if (waitpid(pid, &shell->status, 0) == -1)
			error_free_all(shell, ERR_WAITPID, NULL, NULL);
		read_buffer = try_malloc(shell, sizeof(char));
		while (try_read(shell, pipe_fd[0], &read_buffer, NULL) > 0)
			shell->line_count++;
		set_exit_status_after_termination(shell, &shell->parsing_exit_status, 0);
		set_signals(shell, PARENT_WITHOUT_CHILD);
		try_close(shell, pipe_fd[0]);
	}
}


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
	shell->parsing_exit_status = 2;
}
/*creates a list for each piped token and takes input for each heredoc
if the redirection syntax is wrong, it will print an error*/
void	parse_input(t_minishell *shell)
{
	int				i;
	char			*index;
	t_token_list	*list;

	shell->parsing_exit_status = 0;
	i = 0;
	while (shell->input_array[i])
	{
		list = shell->list[i];
		while (list)
		{
			/* printf("list token: %s list delimiter: %d\n", list->token, list->delimiter); */
			if ((!list->token || !(*list->token)) && list->delimiter != COMMAND)
			{
				error_parsing_input(shell, list, shell->list[i + 1]);
				return ;
			}
			else if (list->delimiter == HEREDOC)
			{
				index = NULL;
				index = ft_itoa(shell, i);
				shell->input_file = append_suffix(shell, index, "_input.txt");
				heredoc(shell, list->token, shell->input_file);
			}
			list = list->next;
		}
		i++;
	}
}
