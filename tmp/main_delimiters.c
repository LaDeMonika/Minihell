#include "inc/minishell.h"
#include "libft/libft.h"
#include <stdbool.h>
#include <unistd.h>

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}
char	*ft_strtrim(char const	*s1, char const	*set)
{
	int		start;
	int		end;
	char	*new;
	int		newlen;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	while (ft_strchr(set, s1[start]))
		start++;
	end = ft_strlen(s1) - 1;
	while (ft_strchr(set, s1[end]))
		end--;
	newlen = end - start + 1;
	if (newlen < 0)
		newlen = 0;
	new = malloc((newlen + 1) * sizeof(char));
	if (!new)
		return (NULL);
	ft_strlcpy(new, s1 + start, newlen + 1);
	*(new + newlen) = '\0';
	return (new);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	char			*sub;
	size_t			slen;

	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	if (len > slen - start)
		len = slen - start;
	if (start >= slen)
		len = 0;
	sub = NULL;
	sub = malloc((len + 1) * sizeof(char));
	if (!sub)
		return (NULL);
	sub[len] = '\0';
	i = 0;
	while (i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	return (sub);
}

int	find_delimiter(char c1, char c2)
{
	if (c1 == '<')
	{
		if (c2 == '<')
		{
			return HEREDOC;
		}
		return INPUT;
	}
	else if (c1 == '>')
	{
		if (c2 == '>')
		{
			return APPEND;
		}
		return OUTPUT;
	}
	return (-1);
}

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
		printf("command_part: %s type: %d\n", current->command_part, current->delimiter);
		current = current->next;

	}
	current->next = new;
}

void	redirect_input(char *input_file)
{
	int	input_fd;

	input_file = ft_strtrim(input_file, " ");
	input_fd = open(input_file, O_RDONLY);

	dup2(input_fd, STDIN_FILENO);
}
/* void	handle_redirections(t_command_list *list, char **envp)
{
	(void)envp;
	while (list)
	{
		if (list->delimiter == INPUT)
			redirect_input(list->)
	}
} */

void	handle_delimiters(char *command, char **envp)
{
	int	i;
	int	preceding_delimiter;
	int	succeeding_delimiter;
	bool	delimiter_found;
	char *command_part;
	t_command_list	*list;
	int	start;
	int	len;

	list = NULL;
	delimiter_found = false;
	i = 0;
	start = 0;
	len = 0;
	preceding_delimiter = -1;
	succeeding_delimiter = -1;

	while (command[i])
	{
		succeeding_delimiter = find_delimiter(command[i], command[i + 1]);
		if (succeeding_delimiter > -1)
		{
			command_part = ft_substr(command, start, len);
			printf("gonna add command part: %s start: %d len: %d\n", command_part, start, len);
			if (!delimiter_found)
			{
				list_add(&list, command_part, COMMAND);
				delimiter_found = true;
			}
			else
			{
				list_add(&list, command_part, preceding_delimiter);
			}
			if (succeeding_delimiter == HEREDOC || succeeding_delimiter == APPEND)
				start = i + 2;
			else
				start = i + 1;
			i = start;
			len = 0;
			preceding_delimiter = succeeding_delimiter;
		}
		else
		{
			len++;
			i++;
		}
	}
	if (i != start)
	{
		command_part = ft_substr(command, start, len);
		printf("gonna add command part: %s start: %d len: %d\n", command_part, start, len);
		list_add(&list, command_part, preceding_delimiter);
	}
	//if no delimiter is found, this function will just return and execute directly
	while (list)
	{
		printf("print command part: %s type: %d\n", list->command_part, list->delimiter);
		list = list->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	/* t_command_list *list;

	list = NULL; */
	//check if handle_delimiters finds command:
	char *command;

	command = argv[1];
	handle_delimiters(command, envp);


}