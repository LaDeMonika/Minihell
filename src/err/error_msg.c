#include "../../inc/minishell.h"
#include <stdlib.h>
#include <unistd.h>

void	free_and_reset(void **ptr)
{
	if (*ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

void	free_all(t_minishell *shell)
{
	free_and_reset((void **)&shell->prompt);
	free_and_reset((void **)&shell->usr_input);
	free_and_reset((void **)shell);
}

void	free_exit(t_minishell *shell, int err)
{
	free_all(shell);
	if (err == ERR_TOO_MANY_ARGS)
		write(STDERR_FILENO, "Too many arguments\nPlease try again\n", 36);
	else if (err == ERR_MALLOC)
		perror("malloc");
	else if (err == ERR_SIGEMPTYSET)
		perror("sigemptyset");
	else if (err == ERR_SIGACTION)
		perror("sigaction");
	else if (err == ERR_PATH_NOT_FOUND)
		perror("getenv");
	else if (err == ERR_OPEN)
		perror("open");
	else if (err == ERR_READ)
		perror("read");
	else if (err == ERR_CLOSE)
		perror("close");
	if (err == NO_ERROR)
		exit(EXIT_SUCCESS);
	exit(EXIT_FAILURE);
}

void	print_error(char *prefix, char *custom_error)
{
	write(STDERR_FILENO, "bash: ", 6);
	write(STDERR_FILENO, prefix, ft_strlen(prefix));
	write(STDERR_FILENO, ": ", 2);
	if (custom_error)
	{
		write(STDERR_FILENO, custom_error, ft_strlen(custom_error));
		write(STDERR_FILENO, "\n", 1);
	}
	else
	{
		perror(NULL);
	}
}
