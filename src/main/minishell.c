/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 15:21:45 by msimic            #+#    #+#             */
/*   Updated: 2024/04/08 11:19:27 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

void execute_command(char *input, t_minishell *shell)
{
	int i;

	shell->input_array = ft_split(input, ' ');
	find_path(shell);
	i = 0;
	while (shell->path_array[i])
	{
		execute_command_path(shell, i);
		i++;
	}
}
/* 
void execute_command(char *input, t_minishell *shell)
{
	char **path_array = ft_split(shell->envp[i] + 5, ':');
	i = 0;
	while (path_array[i])
	{
		shell->command_path = ft_strjoin(path_array[i], "/");
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
		i++;
	}
} */

int main(int argc, char **argv, char **envp)
{
    t_minishell shell;
    char *input;
	(void)argc;
	(void)argv;

    shell.current_dir = getcwd(NULL, 0);
    shell.envp = envp;
    shell.last_exit_status = 0;

    while (1)
    {
		input = readline("minishell> ");
		if (!input)
			break;
        add_history(input);
        execute_command(input, &shell);
        free(input);
    }
    free(shell.current_dir);
    return 0;
}