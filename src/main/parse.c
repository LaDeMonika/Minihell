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
	char	*heredoc_input;
	bool	expand_var;


	try_close(shell, pipe_fd[0]);
	set_signals(shell, HEREDOC_CHILD);
	file_fd = try_open(shell, WRITE_TRUNCATE, input_file);
	if (isatty(fileno(stdin)))
		heredoc_input = readline("> ");
	else
	{
		char *line;
		line = get_next_line(fileno(stdin));
		heredoc_input = ft_strtrim(shell, line, "\n");
	}
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
		if (isatty(fileno(stdin)))
			heredoc_input = readline("> ");
		else
		{
			char *line;
			line = get_next_line(fileno(stdin));
			heredoc_input = ft_strtrim(shell, line, "\n");
		}
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
	char	*read_buffer;

	try_pipe(shell, pipe_fd);
	pid = try_fork(shell);
	if (pid == 0)
	{
		write_to_file(shell, eof, input_file, pipe_fd);

	}

	else
	{
		try_close(shell, pipe_fd[1]);
		set_signals(shell, PARENT_WITH_CHILD);
		if (waitpid(pid, &shell->status, 0) == -1)
			error_free_all(shell, ERR_WAITPID, NULL, NULL);
		read_buffer = try_malloc(shell, sizeof(char));
		while (try_read(shell, pipe_fd[0], &read_buffer, NULL) > 0)
			shell->line_count++;
		/* printf("parsing exit status before setting: %d\n", shell->parsing_exit_status); */
		set_exit_status_after_termination(shell, &shell->parsing_exit_status, 0);
		/* printf("parsing exit status after setting: %d\n", shell->parsing_exit_status); */
		set_signals(shell, PARENT_WITHOUT_CHILD);
		try_close(shell, pipe_fd[0]);
		free_and_reset_ptr((void **)&read_buffer);
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

void	parse_input(t_minishell *shell)
{
	int				i;
	int	j;
	char			*index;
	t_token_list	*list;
	int				exit_status_after_parsing;
	char 	**args;

	shell->parsing_exit_status = 0;
	exit_status_after_parsing = 0;
	i = 0;
	while (shell->input_array[i])
	{
		list = shell->list[i];
		while (list)
		{
			if ((!list->token || !(*list->token)) && list->delimiter != COMMAND)
			{
				error_parsing_input(shell, list, shell->list[i + 1]);
				exit_status_after_parsing = 2;
				break ;
			}
			list = list->next;
		}
		if (shell->parsing_exit_status != 0)
			break ;
		i++;
	}
	j = 0;
	while (shell->input_array[j])
	{
		list = shell->list[j];
		while (list)
		{
			if (shell->pipes_total == 0 && shell->parsing_exit_status == 0 && list->delimiter == COMMAND && list->token)
			{
				if (list->token[0])
				{
					args = split_while_skipping_quotes(shell, list->token, ' ');
					update_value(shell, ft_strdup(shell, "_"), args[sizeof_array((void **)args) - 1], false);
					free_and_reset_array((void ***)&args, false);
					free_and_reset_ptr((void **)&args);
				}
				else
					update_value(shell, ft_strdup(shell, "_"), "", false);

			}

			if (list->delimiter == HEREDOC && j < i)
			{
				/* printf("j: %d\n", j); */

				index = NULL;
				index = ft_itoa(shell, j);
				shell->input_file = append_suffix(shell, index, "_input.txt");
				/* printf("parsing exit status before heredoc: %d\n", shell->parsing_exit_status); */
				heredoc(shell, &list->token, shell->input_file);
				if (shell->parsing_exit_status != 0)
				{
					if (unlink(shell->input_file) == -1)
					{
						free_and_reset_ptr((void **)&shell->input_file);
						error_free_all(shell, ERR_UNLINK, shell->input_file, NULL);
					}

				}
				free_and_reset_ptr((void **)&shell->input_file);

			}
			list = list->next;
		}
		j++;
	}
	if (exit_status_after_parsing == 2 && (shell->parsing_exit_status == 0 || shell->parsing_exit_status == 2))
		shell->parsing_exit_status = 2;
	//printf("parsing exit status: %d\n", shell->parsing_exit_status);
}
