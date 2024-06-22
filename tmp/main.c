#include <stdlib.h>

int	main(void)
{
	char *str = malloc(1024);
	free(str);
	str = NULL;
	if (str)
	{
		free(str);
	}
}