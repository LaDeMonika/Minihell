/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 15:21:45 by msimic            #+#    #+#             */
/*   Updated: 2024/04/09 16:29:40 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
find_path is a helper function that will find the PATH variable in the environment variables.
It will split the PATH variable into an array of strings.
*/
void find_path(t_minishell *shell)
{
	int i;

	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], "PATH=", 5) == 0)
			break;
		i++;
	}
	if (!shell->envp[i])
		return(ft_error_msg(ERR_PATH_NOT_FOUND));
	shell->path_array = ft_split(shell->envp[i] + 5, ':');
}

/*
execute_command_path is a helper function that will execute the command if it exists in the path.
It will fork a child process and execute the command in the child process.
The parent process will wait for the child process to finish.
*/
void	execute_command_path(t_minishell *shell, int i)
{
	shell->command_path = ft_strjoin(shell->path_array[i], "/");
	shell->command_path = ft_strjoin(shell->command_path, shell->input_array[0]);
	shell->fd = access(shell->command_path, F_OK & X_OK);
	if (shell->fd == 0)
	{
		pid_t pid = fork();

		if (pid < 0)
		{
		    // Fork failed
		    perror("fork");
		}
		else if (pid == 0)
		{
		    // Child process
		    execve(shell->command_path, shell->input_array, shell->envp);
		    // If execve returns, it must have failed
		    perror("execve");
		    exit(EXIT_FAILURE);
		}
		else
		{
		    // parent process
		    int status;
		    waitpid(pid, &status, 0);
		}
	}

}

/*
execute_command is the main function that will execute the command.
It will split the input into an array of strings.
It will find the PATH variable in the environment variables.
It will loop through the path array and execute the command if it exists in the path.
*/
void execute_command(char *input, t_minishell *shell)
{
	int i;

	shell->input_array = ft_split(input, ' '); // Split the input into an array of strings using the space character as a delimiter
	find_path(shell); // Find the PATH variable in the environment variables
	i = 0;
	while (shell->path_array[i])
	{
		execute_command_path(shell, i); // Execute the command if it exists in the path
		i++;
	}
}

int main(int ac, char **av, char **envp)
{
    t_minishell *shell = malloc(sizeof(t_minishell));
	if (!shell)
		return (ft_error_msg(ERR_MALLOC), 1);
	(void)ac;
	(void)av;

    init_shell_struct(shell, envp);
    while (1)
    {
		shell->usr_input = readline("minishell> "); // Read the user input
		if (!shell->usr_input)
			break;
        add_history(shell->usr_input); // Add the user input to the history
        execute_command(shell->usr_input, shell); // Execute the command
        free(shell->usr_input);
    }
    free(shell->current_dir);
    return 0;
}
