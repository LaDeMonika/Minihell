#include "inc/minishell.h"

void	list_add(t_command_list **head, char *token, int type)
{
	t_command_list	*new;
	t_command_list *current;

	new = malloc(sizeof(t_command_list));
	new->token = token;
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
	char	*token;
	int	type;

	list = NULL;
	token = "hello";
	type = 0;

	list_add(&list, token, type);
	list_add(&list, "bye", 1);
	list_add(&list, "ok", 2);

	//check function list_add:
	while (list)
	{
		printf("command part: %s type: %d\n", list->token, list->delimiter);
		list = list->next;
	}

}