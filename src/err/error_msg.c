#include "../../inc/minishell.h"
#include <unistd.h>

static void    ft_puterror(const char *fault, const char *msg)
{
    if (fault && ft_strlen(fault))
        printf("Error: %s\n", fault);
    if (msg && ft_strlen(msg))
        printf("Expected: %s\n", msg);
    printf("\n");
}

void    ft_error_msg(char err)
{
    if (err == ERR_MALLOC)
        ft_puterror("Memory allocation failed", "");
    else if (err == ERR_TOO_MANY_ARGS)
        ft_puterror("Too many arguments", "Please try again");
    else if (err == ERR_TOO_FEW_ARGS)
        ft_puterror("Too few arguments", "Please try again");
    else if (err == ERR_INVALID_ARG)
        ft_puterror("Invalid argument", "Please try again");
    else if (err == ERR_PATH_NOT_FOUND)
        ft_puterror("Path not found", "");
    else if (err == NOT_BUILTIN)
        ft_puterror("Command not found", "");
    else
        ft_puterror("Unknown error", "");
}

void    free_all(t_minishell *shell)
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

void	print_error_log(t_minishell *shell)
{
	int	error_log;
	char	*line;

    (void)shell;
    dup2(shell->stderr_copy, STDERR_FILENO);
	error_log = open("error.log", O_RDONLY);
	line = get_next_line(error_log);
	while (line != NULL)
	{
		write(STDERR_FILENO, line, ft_strlen(line) + 1);
		free(line);
		line = get_next_line(error_log);
	}
	close(error_log);
    error_log = open("error.log", O_WRONLY | O_TRUNC);
    close(error_log);
}
void	redirect_errors()
{
	int	stderr_file;

	stderr_file = open("error.log", O_WRONLY | O_APPEND);
	dup2(stderr_file, STDERR_FILENO);
	close(stderr_file);
}
void	redirect_stdout_to_log()
{
	int	stdout_file;

	stdout_file = open("error.log", O_WRONLY | O_APPEND);
	dup2(stdout_file, STDOUT_FILENO);
	close(stdout_file);
}

void	custom_perror(char *prefix, char *custom_message)
{
	write(2, prefix, ft_strlen(prefix));
	write(2, custom_message, ft_strlen(custom_message));
	write(2, "\n", 1);
}
int ft_strlen2(char *s)
{
    int i;

    i = 0;
    while (s && s[i])
    {
        i++;
    }
    return (i);
}

void    append_strerror_to_log(char *prefix, int errnum)
{
    int error_log;
    char    *error_message;

    error_log = open("error.log", O_WRONLY | O_APPEND);
    write(error_log, prefix, ft_strlen(prefix));
    write(error_log, ": ", 2);
    error_message = strerror(errnum);
    write(error_log, error_message, ft_strlen(error_message));
    write(error_log, "\n", 1);
    close(error_log);
}

void    append_custom_error_to_log(char *prefix, char *custom_error)
{
    int error_log;

    error_log = open("error.log", O_WRONLY | O_APPEND);
    write(error_log, prefix, ft_strlen(prefix));
    write(error_log, ": ", 2);
    write(error_log, custom_error, ft_strlen(custom_error));
    write(error_log, "\n", 1);
    close(error_log);
}
