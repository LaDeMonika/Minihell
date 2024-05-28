#include "../../inc/minishell.h"
#include <stdbool.h>

void	parent(t_minishell *shell, char **input_array, int pipes_left,
		int read_fd)
{
	/* printf("parent pid: %d\n", getpid()); */
	set_signals(shell, PARENT_WITH_CHILD);
	try_close(shell, shell->pipe_fd[1]);
	if (read_fd > 0)
		try_close(shell, read_fd);
	if (pipes_left >= 1)
		handle_pipes_recursive(shell, input_array + 1, pipes_left - 1,
			shell->pipe_fd[0]);
	else
	{
		while (shell->pipes_total >= 0)
		{
			if (waitpid(shell->pid[shell->pipes_total], &shell->status, 0) == -1)
				error_free_all(shell, ERR_WAITPID, NULL, NULL);
			set_exit_status_after_termination(shell, &shell->last_exit_status, shell->pipes_total);
			/* printf("child exit status: %d\n", shell->last_exit_status); */
			shell->pipes_total--;
		}
	}
	/* if (pipes_left >= 1)
		try_close(shell, shell->pipe_fd[0]); */
}

void	child(t_minishell *shell, int pipes_left,
		int read_fd)
{
	/* printf("child pid: %d\n", getpid()); */
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
	handle_redirections(shell, shell->list[shell->pipes_total - pipes_left], read_fd);
}
// no pipe: execute directly
// last child: redirect IN, but keep OUT same:
// first child - redirect OUT, but keep IN same:
// middle child - redirect IN and OUT
// if not stdin, close IN of last iteration
// if at least one pipe, fork
// if no pipe (anymore return)
// close all the read ends of pipes before returning from recursive call

void	handle_pipes_recursive(t_minishell *shell, char **input_array,
		int pipes_left, int read_fd)
{
	/* printf("pid: %d\n", getpid()); */
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

