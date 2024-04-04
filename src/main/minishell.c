/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 15:21:45 by msimic            #+#    #+#             */
/*   Updated: 2024/04/04 16:31:05 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void execute_command(char *input, t_minishell *shell)
{
	int i;

	shell->input_array = ft_split(input, ' ');
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], "PATH=", 5) == 0)
			break;
		i++;
	}
	if (!shell->envp[i])
		return(ft_error_msg(ERR_PATH_NOT_FOUND));
	char **path_array = ft_split(shell->envp[i] + 5, ':');
	i = 0;
	while (path_array[i])
	{
		shell->command_path = ft_strjoin(path_array[i], "/");
		shell->command_path = ft_strjoin(shell->command_path, shell->input_array[0]);
		shell->fd = access(shell->command_path, F_OK & X_OK);
		if (shell->fd == 0)
			execve(shell->command_path, shell->input_array, shell->envp);
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