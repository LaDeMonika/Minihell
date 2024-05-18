#include "../../inc/minishell.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	temporary_input_redirect(t_minishell *shell, int read_fd)
{
	char	*temp_index;
	char	*temp_file;
	int		temp_fd;
	char	*buffer;

	temp_index = ft_itoa(shell, read_fd);
	temp_file = append_suffix(shell, temp_index, "_temp_input");
	temp_fd = try_open(shell, WRITE_TRUNCATE, temp_file);
	buffer = try_malloc(shell, sizeof(char) * 1);
	while (try_read(shell, STDIN_FILENO, &buffer, temp_file) > 0)
		try_write(shell, temp_fd, buffer, 1);
	try_close(shell, temp_fd);
	temp_fd = try_open(shell, READ, temp_file);
	free_and_reset_ptr((void **)&temp_file);
	try_dup2(shell, temp_fd, STDIN_FILENO);
	try_close(shell, temp_fd);
}

void	redirect_stream(t_minishell *shell, char *file, int mode, int fd2)
{
	int	fd;

	fd = try_open(shell, mode, file);
	/* printf("open ok\n"); */
	try_dup2(shell, fd, fd2);
	/* printf("dup2 ok\n"); */
	try_close(shell, fd);
	/* printf("close ok\n"); */
}

/*
regarding input & heredoc: only if it is the most right delimiter,
	is _stdin will be true, and thus STDIN for the command should be redirected
*/
void	handle_redirections(t_minishell *shell, t_token_list *list, int read_fd)
{
	char	*command;

	command = NULL;
	(void)read_fd;
	/* if (read_fd > 0)
		temporary_input_redirect(shell, read_fd); */
	while (list)
	{
		/* printf("token: %s delimiter: %d\n", list->token, list->delimiter); */
		if (list->delimiter == COMMAND)
			command = list->token;
		else if (list->delimiter == INPUT)
			redirect_stream(shell, list->token, READ, STDIN_FILENO);
		else if (list->delimiter == OUTPUT)
			redirect_stream(shell, list->token, WRITE_TRUNCATE, STDOUT_FILENO);
		else if (list->delimiter == APPEND)
			redirect_stream(shell, list->token, WRITE_APPEND, STDOUT_FILENO);
		else if (list->delimiter == HEREDOC)
			redirect_stream(shell, shell->input_file, READ, STDIN_FILENO);
		/* printf("again token: %s delimiter: %d\n", list->token, list->delimiter);
		printf("next %p\n", (char *)list->next); */
		list = list->next;
	}

	execute_command(shell, command);
}
