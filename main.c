#include "minishell.h"
#include "libft/libft.h"

void	display_prompt()
{
	char	*user;
	char	*prompt;

	user = getenv("USER");
	prompt = ft_strjoin(user, "@");

	readline(prompt);
}

int	main(void)
{
	display_prompt();
}