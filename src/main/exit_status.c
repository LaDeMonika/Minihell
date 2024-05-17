#include "../../inc/minishell.h"

int	set_exit_status_before_termination(t_minishell *shell, char **custom_message)
{
	(void)shell;
	if (errno == EACCES)
		return (126);
	else if (errno == EFAULT || errno == ENOENT)
	{
		if (errno == EFAULT)
			*custom_message = ("command not found");
		return (127);
	}
	return (EXIT_FAILURE);
}

void	set_exit_status_after_termination(t_minishell *shell, int *child_status,
		int remaining_children)
{
	if (WIFEXITED(shell->status))
	{
		*child_status = WEXITSTATUS(shell->status);
	}
	else if (WIFSIGNALED(shell->status))
	{
		*child_status = WTERMSIG(shell->status) + 128;
		if (WCOREDUMP(shell->status))
		{
			if (remaining_children == 0)
				write(2, "Quit (core dumped)\n", 19);
		}
		else if (remaining_children < 2)
			write(2, "\n", 1);
	}
}