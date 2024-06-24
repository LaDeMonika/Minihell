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
	t_token_list	*current;

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
	while (array && *array && (*array)[i])
	{
		if (is_list)
			free_and_reset_list_contents((t_token_list *)(*array)[i]);
		else
			free(((*array))[i]);
		(*array)[i] = NULL;
		i++;
	}
	free_and_reset_ptr((void **)array);
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
	free_and_reset_ptr((void **)&shell->env_key);
	free_and_reset_ptr((void **)&shell->env_value);
	free_and_reset_ptr((void **)&shell->new_key);
	free_and_reset_ptr((void **)&shell->new_value);
	free_and_reset_array((void ***)&shell->new_envp, false);
	free_and_reset_ptr((void **)&shell->my_pid);
	free_and_reset_array((void ***)&shell->split_array, false);
	free_and_reset_ptr((void **)&shell->path);
	free_and_reset_ptr((void **)&shell->command_path);
	free_and_reset_ptr((void **)&shell->new_pwd);
	free_and_reset_ptr((void **)&shell->temp_str);
	free_and_reset_ptr((void **)&shell->my_pid);
	free_and_reset_ptr((void **)&shell->heredoc_input);
	free_and_reset_ptr((void **)&shell->token);
	free_and_reset_ptr((void **)&shell->last_arg);
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
