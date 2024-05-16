#include "../../inc/minishell.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	redirect_output(t_minishell *shell, char *output_file, int delimiter)
{
	int	output_fd;

	if (delimiter == OUTPUT)
		output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (output_fd == -1)
		error_free_all(shell, ERR_OPEN, output_file, NULL);
	if (dup2(output_fd, STDOUT_FILENO) == -1)
		error_free_all(shell, ERR_DUP2, output_file, NULL);
	if (close(output_fd) == -1)
		error_free_all(shell, ERR_CLOSE, output_file, NULL);
}

void	temporary_input_redirect(t_minishell *shell, int read_fd)
{
	char	*temp_index;
	char	*temp_file;
	int		temp_fd;
	char	*buffer;

	temp_index = ft_itoa(shell, read_fd);
	temp_file = append_suffix(shell, temp_index, "_temp_input");
	temp_fd = try_open(shell, WRITE_TRUNCATE, 0777, temp_file);
	buffer = try_malloc(shell, sizeof(char) * 1);
	while (try_read(shell, STDIN_FILENO, &buffer, temp_file) > 0)
		try_write(shell, 1, temp_fd, buffer);
	try_close(shell, temp_fd);
	temp_fd = try_open(shell, READ, 0, temp_file);
	free_and_reset_ptr((void **)&temp_file);
	try_dup2(shell, temp_fd, STDIN_FILENO);
	try_close(shell, temp_fd);
}

void	redirect_input(t_minishell *shell, char *input_file)
{
	int	input_fd;


	input_fd = try_open(shell, READ, 0, input_file);
	try_dup2(shell, input_fd, STDIN_FILENO);
	try_close(shell, input_fd);

	/* input_fd = open(input_file, O_RDONLY);
	if (input_fd < 0)
		error_free_all(shell, ERR_OPEN, input_file, NULL);
	if (dup2(input_fd, STDIN_FILENO) == -1)
		error_free_all(shell, ERR_DUP2, input_file, NULL);
	if (close(input_fd) == -1)
		error_free_all(shell, ERR_CLOSE, input_file, NULL); */
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
		if (list->delimiter == COMMAND)
			command = list->token;
		else if (list->delimiter == INPUT)
			redirect_input(shell, list->token);
		else if (list->delimiter == OUTPUT)
		{
			redirect_output(shell, list->token, OUTPUT);
		}
		else if (list->delimiter == APPEND)
			redirect_output(shell, list->token, APPEND);
		else if (list->delimiter == HEREDOC)
			redirect_input(shell, shell->input_file);
		list = list->next;
	}
	execute_command(shell, command);
}
