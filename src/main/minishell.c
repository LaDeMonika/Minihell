#include "../../inc/minishell.h"
#include "../../libft/libft.h"
#include <fcntl.h>
#include <linux/limits.h>
#include <readline/history.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char	*build_prompt(void)
{
	char	*prompt;
	char	*user;
	int		fd_hostname;
	int		bytes_read;
	char	hostname_buffer[254];
	int		hostname_len;
	char	*path_temp;
	char	path[PATH_MAX];
	char	*home;

	user = getenv("USER");
	prompt = ft_strjoin(user, "@");
	prompt = ft_strjoin("fake_", prompt);
	fd_hostname = open("/etc/hostname", O_RDONLY);
	bytes_read = read(fd_hostname, hostname_buffer, 254);
	close(fd_hostname);
	hostname_len = strchr(hostname_buffer, '.') - hostname_buffer;
	hostname_buffer[hostname_len] = '\0';
	prompt = ft_strjoin(prompt, hostname_buffer);
	prompt = ft_strjoin(prompt, ":");
	path_temp = getenv("PWD");
	home = getenv("HOME");
	if (ft_strnstr(path_temp, home, ft_strlen(home)))
	{
		ft_strlcpy(path, path_temp + ft_strlen(home), ft_strlen(path_temp)
			- ft_strlen(home) + 1);
		prompt = ft_strjoin(prompt, "~");
	}
	prompt = ft_strjoin(prompt, path_temp);
	prompt = ft_strjoin(prompt, "$ ");
	return (prompt);
}

char	*find_command(char **input_array)
{
	char	*path;
	char	**path_array;
	int		i;
	char	*command_path;
	int		fd;

	path = getenv("PATH");
	if (!path)
		return(ft_error_msg(ERR_PATH_NOT_FOUND), NULL);
	path_array = ft_split(path, ':');
	i = 0;
	while (path_array[i])
	{
		command_path = ft_strjoin(path_array[i], "/");
		command_path = ft_strjoin(command_path, input_array[0]);
		fd = access(command_path, F_OK & X_OK);
		if (fd == 0)
		{
			return (command_path);
		}
		i++;
	}
	return (NULL);
}



void	execute_command(char *command, char **envp)
{
	char	**command_array;
	char	*path;



	command_array = ft_split(command, ' ');
	path = NULL;
	if (strncmp(command_array[0], "./", 2) != 0)
		path = find_command(command_array);
	else
		path = command_array[0];
	if (path)
	{
		execve(path, command_array, envp);
		perror("execve");
	}
	exit(EXIT_FAILURE);
}

int	find_delimiter(char c1, char c2)
{
	if (c1 == '<')
	{
		if (c2 == '<')
		{
			return HEREDOC;
		}
		return INPUT;
	}
	else if (c1 == '>')
	{
		if (c2 == '>')
		{
			return APPEND;
		}
		return OUTPUT;
	}
	return (-1);
}

void	list_add(t_command_list **head, char *command_part, int type)
{
	t_command_list	*new;
	t_command_list	*current;

	new = malloc(sizeof(t_command_list));
	new->command_part = command_part;
	new->delimiter = type;
	new->next = NULL;

	if (!*head)
	{
		*head = new;
		return ;
	}
	current = *head;
	while (current->next)
	{
		current = current->next;
	}
	current->next = new;
}
void	redirect_input(char *input_file)
{
	int	input_fd;

	input_fd = open(input_file, O_RDONLY);
	if (input_fd)
		dup2(input_fd, STDIN_FILENO);
}

void	redirect_output(char *output_file)
{
	int	output_fd;

	output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(output_fd, STDOUT_FILENO);
}

void	append_output(char *output_file)
{
	int	output_fd;

	output_fd = open(output_file, O_WRONLY | O_APPEND);
	dup2(output_fd, STDOUT_FILENO);
}

void	heredoc_input(char *eof)
{
	int input_fd;
	char *input;

	(void)eof;
	input_fd = open("input.txt", O_WRONLY | O_CREAT | O_TRUNC, 0777);
	input = readline("heredoc> ");
	while (ft_strncmp(input, eof, ft_strlen(eof) +1) != 0)
	{
		write(input_fd, input, ft_strlen(input) + 1);
		write(input_fd, "\n", 1);
		input = readline("heredoc> ");
	}
	close(input_fd);
	input_fd = open("input.txt", O_RDONLY);
	dup2(input_fd, STDIN_FILENO);
	close(input_fd);
}

void	handle_redirections(t_command_list *list, char **envp)
{
	char *command;

	command = NULL;
	(void)envp;
	while (list)
	{
		list->command_part = ft_strtrim(list->command_part, " ");
		if (list->delimiter == COMMAND)
		{
			command =list->command_part;
		}
		else if (list->delimiter == INPUT)
		{
			redirect_input(list->command_part);
		}
		else if (list->delimiter == OUTPUT)
		{
			redirect_output(list->command_part);
		}
		else if (list->delimiter == APPEND)
		{
			append_output(list->command_part);
		}
		else if (list->delimiter == HEREDOC)
		{
			heredoc_input(list->command_part);
		}
		list = list->next;
	}
	execute_command(command, envp);
}
/*
find delimiter and add those parts to a linked list with info of delimiter kind
if no delimiter is found, it will only add the command to list
*/
void	handle_delimiters(char *command, char **envp)
{
	int	i;
	int	preceding_delimiter;
	int	succeeding_delimiter;
	bool	delimiter_found;
	char *command_part;
	t_command_list	*list;
	int	start;
	int	len;

	list = NULL;
	delimiter_found = false;
	i = 0;
	start = 0;
	len = 0;
	preceding_delimiter = COMMAND;
	succeeding_delimiter = -1;
	while (command[i])
	{
		succeeding_delimiter = find_delimiter(command[i], command[i + 1]);
		if (succeeding_delimiter > -1)
		{
			command_part = ft_substr(command, start, len);
			if (!delimiter_found)
				delimiter_found = true;
			list_add(&list, command_part, preceding_delimiter);
			if (succeeding_delimiter == HEREDOC || succeeding_delimiter == APPEND)
				start = i + 2;
			else
				start = i + 1;
			i = start;
			len = 0;
			preceding_delimiter = succeeding_delimiter;
		}
		else
		{
			len++;
			i++;
		}
	}
	if (i != start)
	{
		command_part = ft_substr(command, start, len);
		list_add(&list, command_part, preceding_delimiter);
	}
	handle_redirections(list, envp);
}

// no pipe: execute directly
// last child: redirect IN, but keep OUT same:
// first child - redirect OUT, but keep IN same:
// middle child - redirect IN and OUT
// if not stdin, close IN of last iteration
// if at least one pipe, fork
// if no pipe (anymore return)
// close all the read ends of pipes before returning from recursive call
void	handle_pipes(char **input_array, int pipes, char **envp, int read_fd)
{
	int	pipe_fd[2];
	int	pid;
	int	status;

	pipe(pipe_fd);
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
	}
	else if (pid > 0)
	{
		close(pipe_fd[1]);
		if (read_fd > 0)
			close(read_fd);
		if (pipes >= 1)
		{
			handle_pipes(input_array + 1, pipes - 1, envp, pipe_fd[0]);
		}
		else
		{
			while ((waitpid(-1, &status, 0)) > 0)
				;
		}
		close(pipe_fd[0]);
	}
	else if (pid == 0)
	{
		close(pipe_fd[0]);
		if (read_fd > 0)
		{
			dup2(read_fd, STDIN_FILENO);
			close(read_fd);
		}
		if (pipes >= 1)
		{
			dup2(pipe_fd[1], STDOUT_FILENO);
		}
		close(pipe_fd[1]);
		handle_delimiters(input_array[0], envp);
	}
}


void	handle_input(char *input, char **envp)
{
	char	**input_array;
	int		pipes;

	if (strncmp(input, "exit", 5) == 0 || strncmp(input, "exit ", 5) == 0)
		exit(EXIT_SUCCESS);

	input_array = ft_split(input, '|');
	pipes = 0;
	while (input_array[pipes + 1])
		pipes++;
	handle_pipes(input_array, pipes, envp, STDIN_FILENO);
}

void	sigint_handler(int sig, siginfo_t *info, void *ucontext)
{
	(void)sig;
	(void)info;
	(void)ucontext;
	/* printf("\n%s", build_prompt()); */
	printf("\n");
	rl_on_new_line();
    rl_replace_line("", 0); // Clear the current input
    rl_redisplay(); // Redisplay the prompt on a new line
}



int	main(int argc, char **argv, char **envp)
{
	t_minishell *shell;

	char *prompt;
	struct sigaction sa;

	(void)argc;
	(void)argv;

	shell = malloc(sizeof(t_minishell));
	if (!shell)
		return (ft_error_msg(ERR_MALLOC), 1);
	init_shell_struct(shell, envp);

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = sigint_handler;

	sigaction(SIGINT, &sa, NULL);
	while (1)
	{
		prompt = build_prompt();
		shell->usr_input = readline(prompt);

		if (ft_strncmp(shell->usr_input, "\0", 1) != 0)
		{
			add_history(shell->usr_input);
			handle_input(shell->usr_input, envp);
			free(shell->usr_input);
		}
		
	}
	free(shell->current_dir);

}
