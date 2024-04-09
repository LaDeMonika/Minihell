#include "inc/minishell.h"

void	list_add(t_command_list **head, char *command_part, int type)
{
	t_command_list	*new;
	t_command_list *current;

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
		printf("check\n");
	}
	current->next = new;

}

int	main()
{
	t_command_list *list;
	char	*command_part;
	int	type;

	list = NULL;
	command_part = "hello";
	type = 0;

	list_add(&list, command_part, type);
	list_add(&list, "bye", 1);
	list_add(&list, "ok", 2);

	//check function list_add:
	while (list)
	{
		printf("command part: %s type: %d\n", list->command_part, list->delimiter);
		list = list->next;
	}

}