#include "../../inc/minishell.h"


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

void	heredoc_to_file(t_minishell *shell, char **eof, int file_fd, int pipe_fd_write)
{
	bool	expand_env;

	expand_env = true;
	if (ft_strchr(*eof, '"') || ft_strchr(*eof, '\''))
		expand_env = false;
	if (has_even_metaquotes(*eof))
		*eof = remove_metaquotes(shell, *eof);
	// shell->heredoc_input = readline("> "); //uncomment before submitting
	shell->heredoc_input = tester_readline(shell, "> ");
	while (shell->heredoc_input && (ft_strncmp(shell->heredoc_input, *eof,
				ft_strlen(*eof) + 1) != 0))
	{
		if (expand_env)
		{
			shell->heredoc_input = expand_env_variables(shell,
					shell->heredoc_input);
			shell->temp_str = NULL;
		}
		try_write(shell, pipe_fd_write, "\n", 1);
		try_write(shell, file_fd, shell->heredoc_input,
			ft_strlen(shell->heredoc_input));
		try_write(shell, file_fd, "\n", 1);
		free_and_reset_ptr((void **)&shell->heredoc_input);
		shell->heredoc_input = tester_readline(shell, "> ");
		// shell->heredoc_input = readline("> "); //uncomment before submitting
	}
}

#include "../get_next_line/get_next_line.h"
void	heredoc_child(t_minishell *shell, char **eof, char *input_file,
		int pipe_fd[2])
{
	int		file_fd;


	try_close(shell, pipe_fd[0]);
	set_signals(shell, HEREDOC_CHILD);
	file_fd = try_open(shell, input_file, WRITE_TRUNCATE);
	heredoc_to_file(shell, eof, file_fd, pipe_fd[1]);
	if (shell->heredoc_input)
		try_write(shell, pipe_fd[1], "\n", 1);
	if (!shell->heredoc_input)
		heredoc_EOF(shell, *eof);
	try_close(shell, pipe_fd[1]);
	free_all(shell);
	exit(EXIT_SUCCESS);
}


/*heredoc will read input in a child. local line count will increase by 1 for each line and added to global line count in the parent*/
void	heredoc(t_minishell *shell, char **eof, char *input_file)
{
	int		pid;
	int		pipe_fd[2];
	char	buffer[8];
	char	*buffer_ptr;
	int	heredoc_exit_status;

	try_pipe(shell, pipe_fd);
	pid = try_fork(shell);
	if (pid == 0)
		heredoc_child(shell, eof, input_file, pipe_fd);
	else
	{
		try_close(shell, pipe_fd[1]);
		set_signals(shell, PARENT_WITH_CHILD);
		if (waitpid(pid, &shell->status, 0) == -1)
			error_free_all(shell, ERR_WAITPID, NULL, NULL);
		buffer_ptr = buffer;
		while (try_read(shell, pipe_fd[0], &buffer_ptr, NULL) > 0)
			shell->line_count++;
		set_exit_status_after_termination(shell, &heredoc_exit_status,
			0);
		set_signals(shell, PARENT_WITHOUT_CHILD);
		try_close(shell, pipe_fd[0]);
	}
}