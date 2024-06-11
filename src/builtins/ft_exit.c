#include "../../inc/minishell.h"

int	ft_exit(t_minishell *shell, char **command_array)
{
	bool valid_number;
	int	exit_code;

	valid_number = true;
	if (command_array[1])
	{
		exit_code = ft_atoi(command_array[1], &valid_number);
		if (valid_number)
		{
			if (command_array[2])
			{
				errno = U_TOO_MANY_ARGUMENTS;
				return (1);
			}
			shell->last_exit_status = exit_code % 256;
			exit(exit_code % 256);
		}
		else
		{
			print_error(command_array[1], "numeric argument required");
			exit (2);
		}
	}
	exit(0);
}