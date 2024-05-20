#include "../../inc/minishell.h"

void	ft_putstr_fd(char	*s, int fd)
{
	if (!s)
		return ;
	while (*s)
	{
		write(fd, s, 1);

		s++;
	}
}

bool	is_space(char c)
{
	if ((c >= '\t' && c <= '\r') || c == ' ')
		return (true);
	return (false);
}
