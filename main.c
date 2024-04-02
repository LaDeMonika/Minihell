#include "minishell.h"

/* char	*build_prompt()
{
	char	*prompt;
	char	*user;
	int	fd_hostname;
	int	bytes_read;
	char	hostname_buffer[254];
	int	hostname_len;
	char	*path;
	char	*home;
	char	*temp;



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

	path = getenv("PWD");
	home = getenv("HOME");
	temp = ft_strnstr(path, home, ft_strlen(home));
	if (temp)
	{
		ft_strlcpy(path, path + ft_strlen(home), ft_strlen(path) - ft_strlen(home) + 1);
		prompt = ft_strjoin(prompt, "~");
	}
	prompt = ft_strjoin(prompt, path);
	prompt = ft_strjoin(prompt, "$ ");

	return (prompt);
}

void	execute_command(char *input, char **envp)
{
	char	*path;
	char	**path_array;
	char	**input_array;
	int	i;
	char	*command_path;
	int	fd;

	path = getenv("PATH");
	path_array = ft_split(path, ':');
	input_array = ft_split(input, ' ');
	i = 0;
	while (path_array[i])
	{

		command_path = ft_strjoin(path_array[i], "/");
		command_path = ft_strjoin(command_path, input_array[0]);
		fd = access(command_path, F_OK & X_OK);
		if(fd == 0)
		{
			printf("%s\n", input_array[0]);
			printf("%s\n", command_path);
			execve(command_path, input_array, envp);
		}
		i++;
	}
	exit(EXIT_FAILURE);

}

void	display_prompt(int sigint)
{
	char	*prompt;
	char	*input;

	(void)sigint;
	prompt = build_prompt();
	input = readline(prompt);
}

int	main(int argc, char **argv, char **envp)
{
	char	*build_prompt();
	char	*prompt;
	char	*user;
	int	fd_hostname;
	int	bytes_read;
	char	hostname_buffer[254];
	int	hostname_len;
	char	*path;
	char	*home;
	char	*temp;



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

	path = getenv("PWD");
	home = getenv("HOME");
	temp = ft_strnstr(path, home, ft_strlen(home));
	if (temp)
	{
		ft_strlcpy(path, path + ft_strlen(home), ft_strlen(path) - ft_strlen(home) + 1);
		prompt = ft_strjoin(prompt, "~");
	}
	prompt = ft_strjoin(prompt, path);
	prompt = ft_strjoin(prompt, "$ ");

	return (prompt);
}

void	execute_command(char *input, char **envp)
{
	char	*path;
	char	**path_array;
	char	**input_array;
	int	i;
	char	*command_path;
	int	fd;

	path = getenv("PATH");
	path_array = ft_split(path, ':');
	input_array = ft_split(input, ' ');
	i = 0;
	while (path_array[i])
	{

		command_path = ft_strjoin(path_array[i], "/");
		command_path = ft_strjoin(command_path, input_array[0]);
		fd = access(command_path, F_OK & X_OK);
		if(fd == 0)
		{
			printf("%s\n", input_array[0]);
			printf("%s\n", command_path);
			execve(command_path, input_array, envp);
		}
		i++;
	}
	exit(EXIT_FAILURE);

}

void	display_prompt(int sigint)
{
	char	*prompt;
	char	*input;
	int	pid;
	int	status;

	(void)argc;
	(void)argv;



	prompt = build_prompt();

	input = readline(prompt);
	while (input)
	{

		add_history(input);
		pid = fork();
		if (pid == 0)
			execute_command(input, envp);
		wait(&status);
		input = readline(prompt);
	}


	signal(SIGINT, display_prompt);
} */

void execute_command(char *input, t_minishell *shell)
{
    char **input_array;
	int i;
	char *command_path;
	int fd;

	input_array = ft_split(input, ' ');
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], "PATH=", 5) == 0)
			break;
		i++;
	}
	if (!shell->envp[i])
	{
		ft_error_msg(ERR_PATH_NOT_FOUND);
		return;
	}
	char **path_array = ft_split(shell->envp[i] + 5, ':');
	i = 0;
	while (path_array[i])
	{
		command_path = ft_strjoin(path_array[i], "/");
		command_path = ft_strjoin(command_path, input_array[0]);
		fd = access(command_path, F_OK & X_OK);
		if (fd == 0)
		{
			execve(command_path, input_array, shell->envp);
		}
		i++;
	}
	ft_error_msg(ERR_INVALID_ARG);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv, char **envp)
{
    t_minishell shell;
    char *input;
	(void)argc;
	(void)argv;

    shell.current_dir = getcwd(NULL, 0);
    shell.envp = envp;
    shell.last_exit_status = 0;

    while ((input = readline("minishell> ")))
    {
        add_history(input);
        execute_command(input, &shell);
        free(input);
    }

    free(shell.current_dir);
    return 0;
}