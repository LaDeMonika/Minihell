/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 19:33:55 by lilin             #+#    #+#             */
/*   Updated: 2023/09/16 14:25:06 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_list	*create_node(t_list *lst, t_list **head,
void *(*f)(void*), void(*del)(void*))
{
	t_list	*temp;
	void	*content;

	content = f(lst->content);
	if (!content)
	{
		ft_lstclear(head, del);
		return (NULL);
	}
	temp = ft_lstnew(content);
	if (!temp)
	{
		del(content);
		ft_lstclear(head, del);
		return (NULL);
	}
	return (temp);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void*), void(*del)(void*))
{
	t_list	*head;
	t_list	*temp;
	t_list	*current;

	head = NULL;
	temp = NULL;
	current = NULL;
	if (!lst || !f || !del)
		return (NULL);
	while (lst)
	{
		temp = create_node(lst, &head, f, del);
		if (!temp)
			return (NULL);
		if (!head)
			head = temp;
		else
			current->next = temp;
		current = temp;
		lst = lst->next;
	}
	return (head);
}
