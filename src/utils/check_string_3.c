#include "../../inc/minishell.h"

int	index_of_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

int	count_occurences_of_char(char *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == c)
			count++;
		i++;
	}
	return (count);
}

char	*last_word(char *str)
{
	int		i;
	char	*space_index;
	char	*last_word;

	i = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
			i = skip_between_metaquotes(str, i, str[i]);
		i++;
	}
	space_index = strchr(str, ' ');
	if (space_index)
	{
		while (space_index)
		{
			last_word = space_index + 1;
			space_index = strchr(last_word, ' ');
		}
	}
	else
		last_word = str;
	return (last_word);
}
