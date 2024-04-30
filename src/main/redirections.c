#include "../../inc/minishell.h"
#include <stdlib.h>
#include <unistd.h>

void	append_output(char *output_file)
{
	int	output_fd;

	output_fd = open(output_file, O_WRONLY | O_APPEND);
	dup2(output_fd, STDOUT_FILENO);
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
		write(2, " delimited by end-of-file (wanted `eof')\n", 42);
	}
	add_to_line_count(shell, local_line_count);
	close(input_fd);
}



void	redirect_input(char *input_file)
{
	int	input_fd;

	input_fd = open(input_file, O_RDONLY);
	if (input_fd > 0)
		dup2(input_fd, STDIN_FILENO);
	else
	{
		perror(input_file);
		exit(EXIT_FAILURE);
	}
	close(input_fd);
}

void	redirect_output(char *output_file)
{
	int	output_fd;

	output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(output_fd, STDOUT_FILENO);
}

/*
regarding input & heredoc: only if it is the most right delimiter, is _stdin will be true, and thus STDIN for the command should be redirected
*/
void	handle_redirections(t_minishell *shell, t_command_list *list, char **envp)
{
	char *command;

	command = NULL;
	(void)envp;
	while (list)
	{
		if (list->delimiter == COMMAND)
			command = list->command_part;
		else if (list->delimiter == INPUT && list->is_stdin)
			redirect_input(list->command_part);
		else if (list->delimiter == OUTPUT)
			redirect_output(list->command_part);
		else if (list->delimiter == APPEND)
			append_output(list->command_part);
		else if (list->delimiter == HEREDOC)
		{
			heredoc(shell, list->command_part);
			if (list->is_stdin)
				redirect_input("input.txt");
		}
		list = list->next;
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