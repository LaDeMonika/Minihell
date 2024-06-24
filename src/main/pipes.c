/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:36:03 by lilin             #+#    #+#             */
/*   Updated: 2024/06/24 18:36:32 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <stdbool.h>

void	parent(t_minishell *shell, char **input_array, int pipes_left,
		int read_fd)
{
	int	pipes_current;

	set_signals(shell, PARENT_WITH_CHILD);
	try_close(shell, shell->pipe_fd[1]);
	if (read_fd > 0)
		try_close(shell, read_fd);
	if (pipes_left >= 1)
		handle_pipes(shell, input_array + 1, pipes_left - 1, shell->pipe_fd[0]);
	else
	{
		pipes_current = shell->pipes_total;
		while (pipes_current >= 0)
		{
			if (waitpid(shell->pid[pipes_current], &shell->status, 0) == -1)
				error_free_all(shell, ERR_WAITPID, NULL, NULL);
			set_exit_status_after_termination(shell, &shell->last_exit_status,
				pipes_current);
			pipes_current--;
		}
	}
}

void	child(t_minishell *shell, int pipes_left, int read_fd)
{
	set_signals(shell, CHILD);
	try_close(shell, shell->pipe_fd[0]);
	if (read_fd > 0)
	{
		try_dup2(shell, read_fd, STDIN_FILENO);
		try_close(shell, read_fd);
	}
	if (pipes_left >= 1)
		try_dup2(shell, shell->pipe_fd[1], STDOUT_FILENO);
	try_close(shell, shell->pipe_fd[1]);
	handle_redirections(shell, shell->list[shell->pipes_total - pipes_left],
		shell->pipes_total - pipes_left);
}

void	handle_pipes(t_minishell *shell, char **input_array, int pipes_left,
		int read_fd)
{
	if (pipe(shell->pipe_fd) == -1)
		error_free_all(shell, ERR_PIPE, NULL, NULL);
	shell->pid[pipes_left] = fork();
	if (shell->pid[pipes_left] < 0)
		error_free_all(shell, ERR_FORK, NULL, NULL);
	else if (shell->pid[pipes_left] > 0)
		parent(shell, input_array, pipes_left, read_fd);
	else if (shell->pid[pipes_left] == 0)
		child(shell, pipes_left, read_fd);
}
