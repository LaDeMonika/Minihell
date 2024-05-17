#include "inc/minishell.h"

void	list_add(t_str_list **head, char *str, int type)
{
	t_str_list	*new;
	t_str_list *current;

	new = malloc(sizeof(t_str_list));
	new->str = str;
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
	t_str_list *list;
	char	*str;
	int	type;

	list = NULL;
	str = "hello";
	type = 0;

	list_add(&list, str, type);
	list_add(&list, "bye", 1);
	list_add(&list, "ok", 2);

	//check function list_add:
	while (list)
	{
		printf("command part: %s type: %d\n", list->str, list->delimiter);
		list = list->next;
	}

}