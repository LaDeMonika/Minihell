#include "../../inc/minishell.h"
#include <stdlib.h>
#include <unistd.h>


void	redirect_output(char *output_file, int delimiter, char **error)
{
	int	output_fd;

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
			*error = ft_strjoin(output_file, ": ");
			*error = ft_strjoin(*error,strerror(errno));
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
	while (input && ft_strncmp(input, eof, ft_strlen(eof) +1) != 0)
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
		write(2, "bash: warning: here-document at line ", 38);
		write(2, shell->str_line_count, ft_strlen(shell->str_line_count));
		write(2, " delimited by end-of-file (wanted `", 36);
		write(2, eof, ft_strlen(eof));
		write(2, "')\n", 4);
	}
	add_to_line_count(shell, local_line_count);
	close(input_fd);
}



void	redirect_input(char *input_file, bool is_stdin, char **error)
{
	int	input_fd;

	input_fd = open(input_file, O_RDONLY);
	if (input_fd > 0 && is_stdin)
		dup2(input_fd, STDIN_FILENO);
	else if (input_fd < 0)
	{
		if (!(*error))
		{
			*error = ft_strjoin(input_file, ": ");
			*error = ft_strjoin(*error,strerror(errno));
		}
	}
	close(input_fd);
}



/*
regarding input & heredoc: only if it is the most right delimiter, is _stdin will be true, and thus STDIN for the command should be redirected
*/
void	handle_redirections(t_minishell *shell, t_command_list *list, char **envp)
{
	char *command;
	char	*error;

	command = NULL;
	error = NULL;
	(void)envp;
	while (list)
	{
		if (list->delimiter == COMMAND)
			command = list->command_part;
		else if (list->delimiter == INPUT)
			redirect_input(list->command_part, list->is_stdin, &error);
		else if (list->delimiter == OUTPUT)
			redirect_output(list->command_part, OUTPUT, &error);
		else if (list->delimiter == APPEND)
			redirect_output(list->command_part, APPEND, &error);
		else if (list->delimiter == HEREDOC)
		{
			heredoc(shell, list->command_part);
			if (list->is_stdin)
				redirect_input("input.txt", list->is_stdin, &error);
		}
		list = list->next;
	}
	if (error)
	{
		write(2, error, ft_strlen(error));
		write(2, "\n", 1);
		exit(EXIT_FAILURE);
	}
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
