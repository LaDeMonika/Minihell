#include "../../inc/minishell.h"
#include <unistd.h>

/* static void	ft_puterror(const char *fault, const char *msg)
{
	if (fault && ft_strlen(fault))
		printf("Error: %s\n", fault);
	if (msg && ft_strlen(msg))
		printf("Expected: %s\n", msg);
	printf("\n");
} */

static void	ft_puterror(const char *fault, const char *msg)
{
	if (fault && ft_strlen(fault))
		write(STDERR_FILENO, fault, ft_strlen(fault));
	if (msg && ft_strlen(msg))
		write(STDERR_FILENO, msg, ft_strlen(msg));
}

void	ft_error_msg(char err)
{
	if (err == ERR_MALLOC)
		perror("malloc");
	else if (err == ERR_TOO_MANY_ARGS)
		ft_puterror("Too many arguments\n", "Please try again\n");
	/* else if (err == ERR_TOO_FEW_ARGS)
		ft_puterror("Too few arguments", "Please try again");
	else if (err == ERR_INVALID_ARG)
		ft_puterror("Invalid argument", "Please try again");
	else if (err == ERR_PATH_NOT_FOUND)
		ft_puterror("Path not found", "");
	else if (err == NOT_BUILTIN)
		ft_puterror("Command not found", "");
	else
		ft_puterror("Unknown error", ""); */
}

void	free_all(t_minishell *shell)
{
	if (shell->prompt)
		free(shell->prompt);
}

void	error_free_exit(t_minishell *shell, char err)
{
	if (err == ERR_SIGEMPTYSET)
		perror("sigemptyset");
	else if (err == ERR_SIGACTION)
		perror("sigaction");
	else if (err == ERR_OPEN)
		perror("open");
	else if (err == ERR_READ)
		perror("read");
	else if (err == ERR_CLOSE)
		perror("close");
	else if (err == ERR_MALLOC)
		perror("malloc");
	free_all(shell);
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
