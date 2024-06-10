#include "../../inc/minishell.h"

void	temporary_input_redirect(t_minishell *shell, int read_fd)
{
	char	*temp_index;
	char	*temp_file;
	int		temp_fd;
	char	*buffer;

	temp_index = ft_itoa(shell, read_fd);
	temp_file = append_suffix(shell, temp_index, "_temp_input");
	temp_fd = try_open(shell, WRITE_TRUNCATE, temp_file);
	buffer = try_malloc(shell, sizeof(char) * 1);
	while (try_read(shell, STDIN_FILENO, &buffer, temp_file) > 0)
		try_write(shell, temp_fd, buffer, 1);
	try_close(shell, temp_fd);
	temp_fd = try_open(shell, READ, temp_file);
	free_and_reset_ptr((void **)&temp_file);
	try_dup2(shell, temp_fd, STDIN_FILENO);
	try_close(shell, temp_fd);
}

void	redirect_stream(t_minishell *shell, char *file, int mode, int fd2)
{
	int	fd;

	fd = try_open(shell, mode, file);
	try_dup2(shell, fd, fd2);
	try_close(shell, fd);
}

void	handle_redirections(t_minishell *shell, t_token_list *list, int read_fd, int index)
{
	char	*command;
	char			*index_as_str;

	command = NULL;
	(void)read_fd;
	while (list)
	{
		if (list->delimiter == COMMAND)
			command = list->token;
		else if (list->delimiter == INPUT)
			redirect_stream(shell, list->token, READ, STDIN_FILENO);
		else if (list->delimiter == OUTPUT || list->delimiter == FORCE_WRITE)
			redirect_stream(shell, list->token, WRITE_TRUNCATE, STDOUT_FILENO);
		else if (list->delimiter == APPEND)
			redirect_stream(shell, list->token, WRITE_APPEND, STDOUT_FILENO);
		else if (list->delimiter == HEREDOC)
		{
			//printf("trying to redirect from %s\n", shell->input_file);
			index_as_str = NULL;
			index_as_str = ft_itoa(shell, index);
			shell->input_file = append_suffix(shell, index_as_str, "_input.txt");
			redirect_stream(shell, shell->input_file, READ, STDIN_FILENO);
			if (unlink(shell->input_file) == -1)
				error_free_all(shell, ERR_UNLINK, shell->input_file, NULL);
			//printf("unlinked %s\n", shell->input_file);
			free_and_reset_ptr((void **)&shell->input_file);
		}
		list = list->next;
	}
	execute_command(shell, command);
}
