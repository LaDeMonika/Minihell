#include "../../inc/minishell.h"

char	**fill_array_with_null(char **array, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		array[i] = NULL;
		i++;
	}
	return (array);
}

int	sizeof_array(void **array)
{
	int	i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}
