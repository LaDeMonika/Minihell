#include "../../inc/minishell.h"

void	set_exit_status_before_termination(t_minishell *shell, char **custom_message, int *exit_status, int custom_errno)
{
	(void)shell;
	if (custom_errno == U_INVALID_IDENTIFIER)
		*custom_message = "not a valid identifier";
	else if (custom_errno == U_INVALID_OPTION)
		*custom_message = "invalid option";
	else if (custom_errno == U_TOO_MANY_ARGUMENTS)
		*custom_message = "too many arguments";
	else if (custom_errno == U_NO_FILE)
		*custom_message = "No such file or directory";
	else if (custom_errno == U_NO_PERMISSION)
		*custom_message = "Permission denied";
	else if (errno == EFAULT || errno == ENOENT || (errno == EACCES && !shell->command_array[0][0]))
	{
		if (errno == EFAULT || errno == ENOENT || (errno == EACCES && !shell->command_array[0][0]))
			*custom_message = "command not found";
		*exit_status = 127;
	}
	else if (errno == EACCES)
		*exit_status = 126;


}

void	set_exit_status_after_termination(t_minishell *shell, int *child_status,
		int remaining_children)
{
	if (WIFEXITED(shell->status))
		*child_status = WEXITSTATUS(shell->status);
	else if (WIFSIGNALED(shell->status))
	{
		*child_status = WTERMSIG(shell->status) + 128;
		if (WCOREDUMP(shell->status))
		{
			if (remaining_children == 0)
				write(2, "Quit (core dumped)\n", 19);
		}
		else if (*child_status == 130)
			write(2, "\n", 1);
	}
}
