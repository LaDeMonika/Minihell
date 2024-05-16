#include "../../inc/minishell.h"
#include <limits.h>
#include <stdbool.h>

int	try_open(t_minishell *shell, int mode, char *file)
{
	int fd;

	if (mode == WRITE_TRUNCATE)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (mode == WRITE_APPEND)
	 	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(file, O_RDONLY);
	if (fd == -1)
		error_free_all(shell, ERR_OPEN, file, NULL);
	return (fd);
}

int	try_read(t_minishell *shell, int fd, char **buffer, char *file)
{
	int	bytes_read;

	bytes_read = read(fd, *buffer, 1);
	if (bytes_read == -1)
		error_free_all(shell, ERR_READ, file, NULL);
	return (bytes_read);
}

int	try_write(t_minishell *shell, int bytes, int fd, char *buffer)
{
	int	bytes_written;

	bytes_written = write(fd, buffer, bytes);
	if (bytes_written == -1)
		error_free_all(shell, ERR_WRITE, "write", NULL);
	return (bytes_written);
}

void	try_close(t_minishell *shell, int fd)
{
	if (close(fd) == -1)
		error_free_all(shell, ERR_CLOSE, "close", NULL);
}

void	*try_malloc(t_minishell *shell, int size)
{
	void *ptr;

	ptr = malloc(size);
	if (!ptr)
		error_free_all(shell, ERR_MALLOC, NULL, NULL);
	return (ptr);
}

void	try_dup2(t_minishell *shell, int fd, int fd2)
{
	if (dup2(fd, fd2) == -1)
		error_free_all(shell, ERR_DUP2, NULL, NULL);
}