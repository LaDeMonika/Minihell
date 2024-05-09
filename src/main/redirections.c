#include "../../inc/minishell.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	redirect_output(char *output_file, int delimiter)
{
	int	output_fd;

	if (delimiter == OUTPUT)
		output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (output_fd > 0)
	{
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
	else
	{
		print_error(output_file, NULL);
		exit(EXIT_FAILURE);
	}
}

void	temporary_input_redirect(int read_fd)
{
	int		temp_fd;
	char	buffer[1];

	temp_fd = open(ft_strjoin(ft_itoa(read_fd), "_temp_input"),
			O_WRONLY | O_CREAT | O_TRUNC, 0777);
	while ((read(STDIN_FILENO, buffer, 1)) > 0)
		write(temp_fd, buffer, 1);
	close(temp_fd);
	temp_fd = open(ft_strjoin(ft_itoa(read_fd), "_temp_input"), O_RDONLY);
	dup2(temp_fd, STDIN_FILENO);
	close(temp_fd);
}

void	redirect_input(char *input_file, int read_fd)
{
	int	input_fd;

	(void)read_fd;
	input_fd = open(input_file, O_RDONLY);
	if (input_fd > 0)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	else if (input_fd < 0)
	{
		print_error(input_file, NULL);
		exit(EXIT_FAILURE);
	}
}

/*
regarding input & heredoc: only if it is the most right delimiter,
	is _stdin will be true, and thus STDIN for the command should be redirected
*/
void	handle_redirections(t_minishell *shell, t_command_list *list,
		int read_fd)
{
	char	*command;

	command = NULL;
	if (read_fd > 0)
		temporary_input_redirect(read_fd);
	while (list)
	{
		if (list->delimiter == COMMAND)
			command = list->token;
		else if (list->delimiter == INPUT)
			redirect_input(list->token, read_fd);
		else if (list->delimiter == OUTPUT)
		{
			redirect_output(list->token, OUTPUT);
		}
		else if (list->delimiter == APPEND)
			redirect_output(list->token, APPEND);
		else if (list->delimiter == HEREDOC)
			redirect_input(shell->input_file, read_fd);
		list = list->next;
	}
	execute_command(shell, command);
}
