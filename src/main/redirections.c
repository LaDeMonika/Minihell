#include "../../inc/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void	redirect_output(char *output_file, int delimiter, bool *error)
{
	int	output_fd;
	char *error_message;

	if (delimiter == OUTPUT)
		output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
	 	output_fd = open(output_file, O_WRONLY | O_APPEND);
	if (output_fd > 0)
		dup2(output_fd, STDOUT_FILENO);
	else
	{
		if (!(*error))
		{
			*error = true;
			write(STDERR_FILENO, output_file, ft_strlen(output_file) + 1);
			write(STDERR_FILENO, ": ", 3);
			error_message = strerror(errno);
			write(STDERR_FILENO, error_message, ft_strlen(error_message)+ 1);
			write(STDERR_FILENO, "\n", 1);
		}
	}

	close(output_fd);
}

void	heredoc(t_minishell *shell, char *eof)
{
	int input_fd;
	char *input;
	int	local_line_count;
;
	local_line_count = 0;
	input_fd = open("input.txt", O_WRONLY | O_CREAT | O_TRUNC, 0777);

	input = readline("> ");
	while (input && ft_strncmp(input, eof, ft_strlen(eof) + 1) != 0)
	{
		local_line_count++;
		write(input_fd, input, ft_strlen(input));
		write(input_fd, "\n", 1);
		input = readline("> ");
	}
	if (input)
		local_line_count++;
	if (!input)
	{
		dup2(shell->stderr_copy, STDERR_FILENO);
		write(2, "bash: warning: here-document at line ", 38);
		write(2, shell->str_line_count, ft_strlen(shell->str_line_count));
		write(2, " delimited by end-of-file (wanted `", 36);
		write(2, eof, ft_strlen(eof));
		write(2, "')\n", 4);
	}
	add_to_line_count(shell, local_line_count);
	close(input_fd);
}



void	redirect_input(char *input_file, bool is_stdin, bool *error)
{
	int	input_fd;
	char *error_message;

	printf("about to redirect?\n");
	input_fd = open(input_file, O_RDONLY);
	if (input_fd > 0 && is_stdin)
		dup2(input_fd, STDIN_FILENO);
	else if (input_fd < 0)
	{
		printf("opening file failed\n");
		if (!(*error))
		{
			*error = true;
			printf("saving errno to error variable\n");
			write(STDERR_FILENO, input_file, ft_strlen(input_file) + 1);
			write(STDERR_FILENO, ": ", 3);
			error_message = strerror(errno);
			write(STDERR_FILENO, error_message, ft_strlen(error_message) + 1);
			write(STDERR_FILENO, "\n", 1);
		}
	}
	close(input_fd);

}

void	redirect_errors()
{
	int	stderr_file;

	stderr_file = open("error.log", O_WRONLY | O_CREAT | O_TRUNC, 0777);
	dup2(stderr_file, STDERR_FILENO);
	close(stderr_file);
}

/*
regarding input & heredoc: only if it is the most right delimiter, is _stdin will be true, and thus STDIN for the command should be redirected
*/
void	handle_redirections(t_minishell *shell, t_command_list *list, char **envp)
{
	char *command;

	command = NULL;
	shell->error = NULL;
	(void)envp;

	redirect_errors();
	while (list)
	{
		printf("list element: %s\n", list->command_part);
		if (list->delimiter == COMMAND)
			command = list->command_part;
		else if (list->delimiter == INPUT)
			redirect_input(list->command_part, list->is_stdin, &shell->error);
		else if (list->delimiter == OUTPUT)
			redirect_output(list->command_part, OUTPUT, &shell->error);
		else if (list->delimiter == APPEND)
			redirect_output(list->command_part, APPEND, &shell->error);
		else if (list->delimiter == HEREDOC)
		{
			printf("stdin copy fd: %d\n", shell->stdin_copy);
			printf("stderr copy fd: %d\n", shell->stderr_copy);
			dup2(shell->stdin_copy, STDIN_FILENO);
			heredoc(shell, list->command_part);
			if (list->is_stdin)
				redirect_input("input.txt", list->is_stdin, &shell->error);
		}
		list = list->next;
	}
	close(shell->stdin_copy);
	execute_command(shell, command, envp);
}
int	find_delimiter(t_minishell *shell, char c1, char c2)
{
	(void)shell;
	if (c1 == '<')
	{
		if (c2 == '<')
		{
			return HEREDOC;
		}
		return INPUT;
	}
	else if (c1 == '>')
	{
		if (c2 == '>')
			return APPEND;
		return OUTPUT;
	}
	return (-1);
}
