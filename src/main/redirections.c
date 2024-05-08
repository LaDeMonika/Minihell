#include "../../inc/minishell.h"
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
		output_fd = open(output_file, O_WRONLY | O_APPEND);
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

void	redirect_input(char *input_file)
{
	int	input_fd;


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
void	handle_redirections(t_minishell *shell, t_command_list *list)
{
	char	*command;

	command = NULL;
	while (list)
	{
		if (list->delimiter == COMMAND)
			command = list->token;
		else if (list->delimiter == INPUT)
			redirect_input(list->token);
		else if (list->delimiter == OUTPUT)
		{
			redirect_output(list->token, OUTPUT);
		}
		else if (list->delimiter == APPEND)
			redirect_output(list->token, APPEND);
		else if (list->delimiter == HEREDOC)
			redirect_input(shell->input_file);
		list = list->next;
	}
	execute_command(shell, command);
}
