# include "../../inc/minishell.h"
#include <readline/readline.h>

void	child_sigint_handler(int sig)
{
	(void)sig;

	printf("child caught sigint\n");
	exit(EXIT_FAILURE);
}

void	sigint_handler(int sig)
{
	(void)sig;

	/* printf("\n%s", build_prompt()); */

	/* printf("sender pid: %d current pid: %d real uid: %d\n", info->si_pid, getpid(), info->si_uid);
	sender_pid = info->si_pid;
	if (info->si_code == CLD_EXITED) // child:
	{
		printf("Handled by PID: %d, signal from PID: %d real user ID: %d\n", getpid(), info->si_pid, info->si_uid);
		exit(EXIT_FAILURE);
	}
	else //parent: sid > 0 uid > 0
	{
		printf("Handled by PID: %d, signal from PID: %d real user ID: %d\n", getpid(), info->si_pid, info->si_uid);
		//exit(EXIT_FAILURE);
	} */


	//
	//printf("\n");
	printf("parent caught sigint\n");
	write(2, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
    rl_redisplay();
}

void	handle_signals(t_minishell *shell)
{
	/* struct sigaction	sa_sigint; */
	struct sigaction	sa_sigquit;


	shell->sa_sigint.sa_handler = sigint_handler;
	shell->sa_sigint.sa_flags = 0;
	sigemptyset(&shell->sa_sigint.sa_mask);
	sigaction(SIGINT, &shell->sa_sigint, NULL);

	sa_sigquit.sa_handler = SIG_IGN;
	sa_sigquit.sa_flags = 0;
	sigemptyset(&sa_sigquit.sa_mask);
	sigaction(SIGQUIT, &sa_sigquit, NULL);
}
