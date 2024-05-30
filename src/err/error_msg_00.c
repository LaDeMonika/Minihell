/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_msg_00.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 14:13:25 by msimic            #+#    #+#             */
/*   Updated: 2024/05/24 14:19:31 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_and_reset_ptr(void **ptr)
{

	if (ptr && *ptr)
	{
		/* printf("final: freeing pointer %p\n", *ptr); */
		free(*ptr);
		*ptr = NULL;
	}
}
void	free_and_reset_list_contents(t_token_list *list)
{
	while (list)
	{
		free_and_reset_ptr((void **)&list->token);
		list = (list)->next;

	}
}


void	free_and_reset_array(void ***array, bool is_list)
{
	int	i;

	i = 0;
	while (array && *array && (*array)[i])
	{
		if (is_list)
			free_and_reset_list_contents((t_token_list *)(*array)[i]);
		/* printf("freeing %p\n", (*array)[i]); */
		free(((*array))[i]);
		(*array)[i] = NULL;
		i++;
	}

}


/* void	free_and_reset_int(int **ptr)
{
	if (ptr && *ptr)
	{
		free(ptr);
		*ptr = NULL;
	}
} */
void	free_child(t_minishell *shell)
{
	free_and_reset_ptr((void **)&shell->prompt);
	free_and_reset_ptr((void **)&shell->usr_input);
	/* printf("pid: %d\n", getpid());
	printf("address of input_array about to be freed: %p\n", (void *)shell->input_array); */
	free_and_reset_array((void ***)&shell->input_array, false);
	//free_and_reset_list_contents(shell->list);
	free_and_reset_array((void ***)&shell->list, true);
	free_and_reset_ptr((void **)shell->list);
	free_and_reset_ptr((void **)&shell->input_file);
	free_and_reset_array((void ***)&shell->path_array, false);
	free_and_reset_ptr((void **)&shell->path_array);
	free_and_reset_ptr((void **)&shell->pid);
}

void	free_iteration(t_minishell *shell)
{
	free_and_reset_ptr((void **)&shell->prompt);
	free_and_reset_ptr((void **)&shell->usr_input);
	/* printf("pid: %d\n", getpid());
	printf("address of input_array about to be freed: %p\n", (void *)shell->input_array); */
	free_and_reset_array((void ***)&shell->input_array, false);
	free_and_reset_ptr((void **)&shell->input_array);
	/* free_and_reset_list(shell->list);
	free_and_reset_ptr((void **)shell->list); */

	free_and_reset_array((void ***)&shell->list, true);
	free_and_reset_ptr((void **)&shell->list);
	free_and_reset_ptr((void **)&shell->input_file);
	free_and_reset_array((void ***)&shell->path_array, false);
	free_and_reset_ptr((void **)&shell->path_array);
	free_and_reset_array((void ***)&shell->command_array, false);
	free_and_reset_ptr((void **)&shell->command_array);
	free_and_reset_ptr((void **)&shell->pid);
	/* free_and_reset_int(&shell->pid);*/
}

int	free_all(t_minishell *shell)
{
	int	last_exit_status;

	last_exit_status = shell->last_exit_status;
	free_iteration(shell);
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
	else if (err == ERR_PIPE)
		perror("pipe");
	else if (err == ERR_FORK)
		perror("fork");
	else if (err == ERR_WAITPID)
		perror("waitpid");
	else if (err == ERR_GETPID)
		write(STDERR_FILENO, "Error getting pid\n", 18);
}

void	error_free_all(t_minishell *shell, int err,
			char *prefix, char *custom_error)
{
	(void)custom_error;
	free_all(shell);
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
	error_free_all_second(err, prefix);
	if (err == EXIT)
		exit(253);
	exit(EXIT_FAILURE);
}
