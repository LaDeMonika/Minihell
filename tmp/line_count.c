#include "../../inc/minishell.h"
/*
void	read_line_count(t_minishell *shell)
{
	int	line_count_fd;
	int	bytes_read;

	line_count_fd = open("line_count.txt", O_RDONLY);
	bytes_read = read(line_count_fd, shell->str_line_count, 16);
	shell->line_count = ft_atoi(shell->str_line_count);
	close(line_count_fd);
}

void	add_to_line_count(t_minishell *shell, int new_lines)
{
	int		line_count_fd;

	shell->line_count += new_lines;
	shell->str_line_count = ft_itoa(shell->line_count);
	line_count_fd = open("line_count.txt", O_WRONLY);
	write(line_count_fd, shell->str_line_count, ft_strlen(shell->str_line_count));
	close(line_count_fd);
}

void	init_line_count(t_minishell *shell)
{
	int	line_count_fd;
	int	bytes_written;

	line_count_fd = open("line_count.txt", O_WRONLY | O_CREAT | O_TRUNC, 0777);
	shell->line_count = 0;
	shell->str_line_count = "0";
	bytes_written = write(line_count_fd, shell->str_line_count, 1);
	close(line_count_fd);
} */