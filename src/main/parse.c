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
//variation for tester:
#include "../get_next_line/get_next_line.h"
void	write_to_file(t_minishell *shell, char **eof, char *input_file,
		int pipe_fd[2])
{
	int		file_fd;
	//char	*heredoc_input;
	bool	expand_var;


	try_close(shell, pipe_fd[0]);
	set_signals(shell, HEREDOC_CHILD);
	file_fd = try_open(shell, input_file, WRITE_TRUNCATE);
	if (isatty(fileno(stdin)))
		shell->heredoc_input = readline("> ");
	else
	{
		char *line;
		line = get_next_line(fileno(stdin));
		shell->heredoc_input = ft_strtrim(shell, line, "\n");
	}
	if (ft_strchr(*eof, '"') || ft_strchr(*eof, '\''))
		expand_var = false;
	else
		expand_var = true;
	if (has_even_metaquotes(*eof))
		*eof = remove_metaquotes(shell, *eof);
	while (shell->heredoc_input && (ft_strncmp(shell->heredoc_input, *eof, ft_strlen(*eof) + 1) != 0))
	{
		if (expand_var)
		{
			shell->heredoc_input = expand_env_variables(shell, shell->heredoc_input);
			shell->expanded_input = NULL;
		}
		try_write(shell, pipe_fd[1], "\n", 1);
		try_write(shell, file_fd, shell->heredoc_input, ft_strlen(shell->heredoc_input));
		try_write(shell, file_fd, "\n", 1);
		free_and_reset_ptr((void **)&shell->heredoc_input);
		if (isatty(fileno(stdin)))
			shell->heredoc_input = readline("> ");
		else
		{
			char *line;
			line = get_next_line(fileno(stdin));
			shell->heredoc_input = ft_strtrim(shell, line, "\n");
		}
	}
	if (shell->heredoc_input)
		try_write(shell, pipe_fd[1], "\n", 1);
	if (!shell->heredoc_input)
		heredoc_EOF(shell, *eof);
	try_close(shell, pipe_fd[1]);
	//free_and_reset_ptr((void **)&heredoc_input);
	free_all(shell);
	exit(EXIT_SUCCESS);
}


/*writes input to input_file and sends for each input a newline character to pipe to increment line count*/
/* void	write_to_file(t_minishell *shell, char **eof, char *input_file,
		int pipe_fd[2])
{
	int		file_fd;
	char	*heredoc_input;
	bool	expand_var;


	try_close(shell, pipe_fd[0]);
	set_signals(shell, HEREDOC_CHILD);
	file_fd = try_open(shell, WRITE_TRUNCATE, input_file);
	heredoc_input = readline("> ");
	if (ft_strchr(*eof, '"') || ft_strchr(*eof, '\''))
		expand_var = false;
	else
		expand_var = true;
	if (has_even_metaquotes(*eof))
		*eof = remove_metaquotes(shell, *eof);
	while (heredoc_input && (ft_strncmp(heredoc_input, *eof, ft_strlen(*eof) + 1) != 0))
	{
		if (expand_var)
			heredoc_input = expand_env_variables(shell, heredoc_input);
		try_write(shell, pipe_fd[1], "\n", 1);
		try_write(shell, file_fd, heredoc_input, ft_strlen(heredoc_input));
		try_write(shell, file_fd, "\n", 1);
		free_and_reset_ptr((void **)&heredoc_input);
		heredoc_input = readline("> ");
	}
	if (heredoc_input)
		try_write(shell, pipe_fd[1], "\n", 1);
	if (!heredoc_input)
		heredoc_EOF(shell, *eof);
	try_close(shell, pipe_fd[1]);
	free_and_reset_ptr((void **)&heredoc_input);
	free_all(shell);
	exit(EXIT_SUCCESS);
}
 */
/*heredoc will read input in a child. local line count will increase by 1 for each line and added to global line count in the parent*/
void	heredoc(t_minishell *shell, char **eof, char *input_file)
{
	int		pid;
	int		pipe_fd[2];
	char	buffer[8];
	char	*buffer_ptr;

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
		buffer_ptr = buffer;
		while (try_read(shell, pipe_fd[0], &buffer_ptr, NULL) > 0)
			shell->line_count++;
		/* printf("parsing exit status before setting: %d\n", shell->parsing_exit_status); */
		set_exit_status_after_termination(shell, &shell->parsing_exit_status, 0);
		/* printf("parsing exit status after setting: %d\n", shell->parsing_exit_status); */
		set_signals(shell, PARENT_WITHOUT_CHILD);
		try_close(shell, pipe_fd[0]);

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
	write(STDERR_FILENO, shell->unexpected_token,
		ft_strlen(shell->unexpected_token));
	write(STDERR_FILENO, "\n", 1);
	shell->parsing_exit_status = 2;
}

int	parse_token(t_minishell *shell, t_token_list *list, int *exit_status)
{
	int				i;


	shell->parsing_exit_status = 0;
	*exit_status= 0;
	i = 0;
	while (shell->input_array[i])
	{
		list = shell->list[i];
		while (list)
		{
			if ((!list->token || !(*list->token)) && list->delimiter != COMMAND)
			{
				error_parsing_input(shell, list, shell->list[i + 1]);
				*exit_status = 2;
				return (i) ;
			}
			list = list->next;
		}
		i++;
	}
	return (i);
}

void	handle_heredoc(t_minishell *shell, t_token_list *list, int error_at_index)
{
	int	j;
	char			*index;

	j = 0;
	while (shell->input_array[j])
	{
		list = shell->list[j];
		while (list)
		{
			if (list->delimiter == HEREDOC && j < error_at_index)
			{
				index = NULL;
				index = ft_itoa(shell, j);
				shell->input_file = append(shell, index, "_input.txt", FREE_BASE);
				heredoc(shell, &list->token, shell->input_file);
				if (shell->parsing_exit_status != 0)
				{
					if (unlink(shell->input_file) == -1)
					{
						//free_and_reset_ptr((void **)&shell->input_file);
						error_free_all(shell, ERR_UNLINK, shell->input_file, NULL);
					}
				}
				free_and_reset_ptr((void **)&shell->input_file);

			}
			list = list->next;
		}
		j++;
	}
}

void	parse_input(t_minishell *shell)
{
	int	i;
	int				error_at_index;
	int				exit_status_after_parsing;
	t_token_list	*list;

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
	list = NULL;
	error_at_index = parse_token(shell, list, &exit_status_after_parsing);
	handle_heredoc(shell, list, error_at_index);
	if (exit_status_after_parsing == 2 && (shell->parsing_exit_status == 0 || shell->parsing_exit_status == 2))
		shell->parsing_exit_status = 2;
}
