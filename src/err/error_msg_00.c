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

void	free_and_reset_ptr(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}
void	free_and_reset_list_contents(t_token_list *list)
{
	t_token_list *current;

	while (list)
	{
		current = list;
		list = (list)->next;
		if (current->token)
			free_and_reset_ptr((void **)&current->token);
		if (current)
			free_and_reset_ptr((void **)&current);
	}
}


void	free_and_reset_array(void ***array, bool is_list)
{
	int	i;

	i = 0;
	/* if (array)
	{
		printf("array address: %p content: %s\n", array, (char *)array);
		if (*array)
		{
			printf("*array address: %p content: %s\n", *array, (char *)*array);
			if ((*array)[0])
				printf("*array[0] address: %p content: %s\n", (*array)[0], (char *)(*array)[0]);
		}

	} */
	while (array && *array && (*array)[i])
	{
		/* printf("*array[i] address: %p content: %s\n", (*array)[i], (char *)(*array)[i]); */
		if (is_list)
			free_and_reset_list_contents((t_token_list *)(*array)[i]);
		else
			free(((*array))[i]);
		(*array)[i] = NULL;
		i++;
	}
	free_and_reset_ptr((void **)array);
	/* printf("i: %d\n", i); */
}



void	free_iteration(t_minishell *shell)
{
	free_and_reset_ptr((void **)&shell->prompt);
	free_and_reset_ptr((void **)&shell->usr_input);
	free_and_reset_array((void ***)&shell->input_array, false);
	free_and_reset_array((void ***)&shell->list, true);
	free_and_reset_ptr((void **)&shell->input_file);
	free_and_reset_array((void ***)&shell->path_array, false);
	free_and_reset_array((void ***)&shell->command_array, false);
	free_and_reset_ptr((void **)&shell->pid);
	free_and_reset_array((void ***)&shell->env_subarray, false);
}

int	free_all(t_minishell *shell)
{
	int	last_exit_status;

	last_exit_status = shell->last_exit_status;
	free_iteration(shell);
	free_and_reset_array((void ***)&shell->envp, false);
	free_and_reset_ptr((void **)&shell);
	return (last_exit_status);
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

void	error_free_all(t_minishell *shell, int err,
			char *prefix, char *custom_message)
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
		print_error(prefix, NULL);
	else if (err == ERR_DUP2)
		print_error(prefix, NULL);
	else
		error_free_all_second(err, prefix);
	free_all(shell);
	exit(EXIT_FAILURE);
}


/* void	error_free_all_exit(t_minishell *shell, int err, char *prefix, char *custom_message)
{
	error_free_all(shell, err, prefix, custom_message);
	exit(EXIT_FAILURE);
} */