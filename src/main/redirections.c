/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:32:35 by lilin             #+#    #+#             */
/*   Updated: 2024/06/24 18:32:36 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	redirect_stream(t_minishell *shell, char *file, int mode, int fd2)
{
	int	fd;

	fd = try_open(shell, file, mode);
	try_dup2(shell, fd, fd2);
	try_close(shell, fd);
}

void	handle_redirections(t_minishell *shell, t_token_list *list, int index)
{
	char	*command;

	shell->post_sep = COMMAND;
	while (list)
	{
		if (list->sep == COMMAND)
			command = list->token;
		else if (list->sep == INPUT)
			redirect_stream(shell, list->token, READ, STDIN_FILENO);
		else if (list->sep == OUTPUT || list->sep == FORCEWRITE)
			redirect_stream(shell, list->token, WRITE_TRUNCATE, STDOUT_FILENO);
		else if (list->sep == APPEND)
			redirect_stream(shell, list->token, WRITE_APPEND, STDOUT_FILENO);
		else if (list->sep == HEREDOC)
		{
			shell->input_file = append(shell, ft_itoa(shell, index),
					"_input.txt", BASE);
			redirect_stream(shell, shell->input_file, READ, STDIN_FILENO);
			if (unlink(shell->input_file) == -1)
				error_free_all(shell, ERR_UNLINK, shell->input_file, NULL);
			free_and_reset_ptr((void **)&shell->input_file);
		}
		list = list->next;
	}
	execute_command(shell, command);
}
