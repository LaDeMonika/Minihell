#include "../../inc/minishell.h"

int	ft_exit(t_minishell *shell, int *custom_errno)
{
	bool valid_number;
	int	exit_status;

	printf("exit\n");
	valid_number = true;
	if (shell->command_array[1])
	{
		exit_status = ft_atoi(shell->command_array[1], &valid_number);
		if (valid_number)
		{
			if (shell->command_array[2])
			{
				*custom_errno = U_TOO_MANY_ARGUMENTS;
				return (1);
			}
			/* shell->last_exit_status = exit_status % 256;
			free_all(shell);
			exit(exit_status % 256); */
			shell->stay_in_parent = false;
			return (exit_status % 256);
		}
		else
		{
			print_error(shell->command_array[1], "numeric argument required");
			*custom_errno = U_NUMERIC_ARGUMENT_REQUIRED;
			shell->stay_in_parent = false;
			return (2);
		}
	}
	shell->stay_in_parent = false;
	return (0);
}
