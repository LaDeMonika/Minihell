#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int	main(void)
{
	int	pid;
	int	x;

	x = 0;
	pid = fork();
	if (pid == 0)
	{
		x += 5;
		exit(1);
	}
	sleep(2);
	printf("x: %d\n", x);
}