# include "../../inc/minishell.h"
#include <readline/readline.h>
#include <unistd.h>

void	child_sigint_handler(int sig)
{
	(void)sig;

	//printf("child caught sigint\n");
	write(2, "child caught signal\n", 21);
	exit(130);
}

void	child_sigquit_handler(int sig)
{
	(void)sig;

	write(2, "^\\Quit (core dumped)\n", 22);
	exit(131);
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
	//while (kill(0, sig) != -1);

	write(2, "\n", 1);
	//write(2, "parent caught signal\n", 22);
	rl_on_new_line();
	rl_replace_line("", 0);
    rl_redisplay();
}

void	set_child_signals(t_minishell *shell)
{
	shell->sa_sigint.sa_handler = child_sigint_handler;
	sigaction(SIGINT, &shell->sa_sigint, NULL);
	shell->sa_sigquit.sa_handler = child_sigquit_handler;
	sigaction(SIGQUIT, &shell->sa_sigquit, NULL);
}

void	set_last_exit_status(t_minishell *shell)
{
	if (WIFEXITED(shell->status))
	{
		shell->last_exit_status = WEXITSTATUS(shell->status);
	}
	else if (WIFSIGNALED(shell->status))
	{
		write(2, "\n", 1);
		printf("before adding 128: %d\n", WTERMSIG(shell->status));
		shell->last_exit_status = WTERMSIG(shell->status) + 128;
	}
	printf("exit status: %d\n", shell->last_exit_status);
	if (WCOREDUMP(shell->status))
	{

		printf("some core was dumped\n");
		write(2, "^\\Quit (core dumped)\n", 22);
	}
}

void	handle_signals(t_minishell *shell)
{
	shell->sa_sigint.sa_handler = sigint_handler;
	shell->sa_sigint.sa_flags = 0;
	sigemptyset(&shell->sa_sigint.sa_mask);
	sigaction(SIGINT, &shell->sa_sigint, NULL);

	shell->sa_sigquit.sa_handler = SIG_IGN;
	shell->sa_sigquit.sa_flags = 0;
	sigemptyset(&shell->sa_sigquit.sa_mask);
	sigaction(SIGQUIT, &shell->sa_sigquit, NULL);
}
