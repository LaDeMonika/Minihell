/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_msg_00.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 14:13:25 by msimic            #+#    #+#             */
/*   Updated: 2024/05/31 17:18:13 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_error(char *prefix, char *custom_message)
{
	write(STDERR_FILENO, prefix, ft_strlen(prefix));
	write(STDERR_FILENO, ": ", 2);
	if (custom_message)
	{
		write(STDERR_FILENO, custom_message, ft_strlen(custom_message));
		write(STDERR_FILENO, "\n", 1);
	}
	else
		perror(NULL);
}

void	error_free_all_exit(t_minishell *shell, int exit_status)
{
	free_all(shell);
	exit(exit_status);
}

void	error_free_all_second(int err, char *prefix)
{
	if (err == ERR_READ)
		perror("read");
	else if (err == ERR_WRITE)
		print_error(prefix, NULL);
	else if (err == ERR_CLOSE)
		perror("close");
	else if (err == ERR_UNLINK)
		perror("unlink");
	else if (err == ERR_PIPE)
		perror("pipe");
	else if (err == ERR_FORK)
		perror("fork");
	else if (err == ERR_WAITPID)
		perror("waitpid");
	else if (err == ERR_GETPID)
		write(STDERR_FILENO, "Error getting pid\n", 18);
	else if (err == ERR_GETCWD)
		perror("getcwd");
	else if (err == ERR_STAT)
		perror("stat");
}

void	error_free_all(t_minishell *shell, int err, char *prefix,
		char *custom_message)
{
	(void)custom_message;
	if (err == ERR_TOO_MANY_ARGS)
		write(STDERR_FILENO, "Too many arguments\nPlease try again\n", 36);
	else if (err == ERR_MALLOC)
		perror("malloc");
	else if (err == ERR_SIGEMPTYSET)
		perror("sigemptyset");
	else if (err == ERR_SIGACTION)
		perror("sigaction");
	else if (err == ERR_PATH_NOT_FOUND)
		perror("getenv");
	else if (err == ERR_OPEN)
		print_error(prefix, custom_message);
	else if (err == ERR_DUP2)
		print_error(prefix, custom_message);
	else
		error_free_all_second(err, prefix);
	free_all(shell);
	exit(EXIT_FAILURE);
}
