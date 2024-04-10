/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 16:36:05 by msimic            #+#    #+#             */
/*   Updated: 2024/04/10 16:43:44 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	append_output(char *output_file)
{
	int	output_fd;

	output_fd = open(output_file, O_WRONLY | O_APPEND);
	dup2(output_fd, STDOUT_FILENO);
}

void	heredoc_input(char *eof)
{
	int input_fd;
	char *input;

	(void)eof;
	input_fd = open("input.txt", O_WRONLY | O_CREAT | O_TRUNC, 0777);
	input = readline("heredoc> ");
	while (ft_strncmp(input, eof, ft_strlen(eof) +1) != 0)
	{
		write(input_fd, input, ft_strlen(input) + 1);
		write(input_fd, "\n", 1);
		input = readline("heredoc> ");
	}
	close(input_fd);
	input_fd = open("input.txt", O_RDONLY);
	dup2(input_fd, STDIN_FILENO);
	close(input_fd);
}

int	find_delimiter(char c1, char c2)
{
	if (c1 == '<')
	{
		if (c2 == '<')
			return HEREDOC;
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

void	redirect_input(char *input_file)
{
	int	input_fd;

	input_fd = open(input_file, O_RDONLY);
	if (input_fd)
		dup2(input_fd, STDIN_FILENO);
}

void	redirect_output(char *output_file)
{
	int	output_fd;

	output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(output_fd, STDOUT_FILENO);
}

void	handle_redirections(t_command_list *list, char **envp)
{
	char *command;

	command = NULL;
	(void)envp;
	while (list)
	{
		list->command_part = ft_strtrim(list->command_part, " ");
		if (list->delimiter == COMMAND)
			command =list->command_part;
		else if (list->delimiter == INPUT)
			redirect_input(list->command_part);
		else if (list->delimiter == OUTPUT)
			redirect_output(list->command_part);
		else if (list->delimiter == APPEND)
			append_output(list->command_part);
		else if (list->delimiter == HEREDOC)
			heredoc_input(list->command_part);
		list = list->next;
	}
	execute_command(command, envp);
}