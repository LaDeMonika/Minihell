#include "../../inc/minishell.h"
#include <stdbool.h>

void	parent(t_minishell *shell, char **input_array, int pipes_left,
		int read_fd)
{
	set_signals(shell, PARENT_WITH_CHILD);
	close(shell->pipe_fd[1]);
	if (read_fd > 0)
		close(read_fd);
	if (pipes_left >= 1)
	{
		handle_pipes_recursive(shell, input_array + 1, pipes_left - 1,
			shell->pipe_fd[0]);
	}
	else
	{
		while ((waitpid(shell->pid[shell->pipes_total], &shell->status, 0)) > 0)
		{
			set_child_exit_status(shell, &shell->last_exit_status, shell->pipes_total);
			shell->pipes_total--;
		}
	}
	close(shell->pipe_fd[0]);
}

void	child(t_minishell *shell, int pipes_left,
		int read_fd)
{
	set_signals(shell, CHILD);
	close(shell->pipe_fd[0]);
	if (read_fd > 0)
	{
		dup2(read_fd, STDIN_FILENO);
		close(read_fd);
	}
	if (pipes_left >= 1)
	{
		dup2(shell->pipe_fd[1], STDOUT_FILENO);
	}
	close(shell->pipe_fd[1]);
	handle_redirections(shell, shell->list[shell->pipes_total - pipes_left], read_fd);
}

void	handle_pipes_recursive(t_minishell *shell, char **input_array,
		int pipes_left, int read_fd)
{
	pipe(shell->pipe_fd);
	shell->pid[pipes_left] = fork();
	if (shell->pid[pipes_left] < 0)
	{
		perror("fork");
	}
	else if (shell->pid[pipes_left] > 0)
	{
		parent(shell, input_array, pipes_left, read_fd);
	}
	else if (shell->pid[pipes_left] == 0)
	{
		child(shell, pipes_left, read_fd);
	}
}

// no pipe: execute directly
// last child: redirect IN, but keep OUT same:
// first child - redirect OUT, but keep IN same:
// middle child - redirect IN and OUT
// if not stdin, close IN of last iteration
// if at least one pipe, fork
// if no pipe (anymore return)
// close all the read ends of pipes before returning from recursive call
void	handle_pipes(t_minishell *shell, int read_fd)
{
	shell->pid = malloc(sizeof(int) * (shell->pipes_total + 2));
	handle_pipes_recursive(shell, shell->input_array, shell->pipes_total,
		read_fd);

}
