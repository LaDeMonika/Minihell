#include "../../inc/minishell.h"

void	child_sigint_handler(int sig)
{
	(void)sig;
	write(2, "child caught sigint\n", 20);
	write(2, "> ^C\n", 5);
	exit(130);
}

void	child_sigquit_handler(int sig)
{
	(void)sig;
	write(2, "^\\Quit (core dumped)\n", 21);
	exit(131);
}

void	parent_sigint_handler(int sig)
{
	(void)sig;
	write(2, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	set_child_exit_status(t_minishell *shell, int *child_status,
		int remaining_children)
{
	if (WIFEXITED(shell->status))
		*child_status = WEXITSTATUS(shell->status);
	else if (WIFSIGNALED(shell->status))
	{
		*child_status = WTERMSIG(shell->status) + 128;
		if (WCOREDUMP(shell->status) && remaining_children == 0)
			write(2, "Quit (core dumped)\n", 19);
	}

}

void	set_signals(t_minishell *shell, int mode)
{
	if (mode == PARENT_NO_CHILD)
	{
		shell->sa_sigint.sa_handler = parent_sigint_handler;
		shell->sa_sigint.sa_flags = 0;
		if (sigemptyset(&shell->sa_sigint.sa_mask) == -1)
			error_free_exit(shell, ERR_SIGEMPTYSET);
		if (sigaction(SIGINT, &shell->sa_sigint, NULL) == -1)
			error_free_exit(shell, ERR_SIGACTION);
		shell->sa_sigquit.sa_handler = SIG_IGN;
		shell->sa_sigquit.sa_flags = 0;
		if (sigemptyset(&shell->sa_sigquit.sa_mask) == -1)
			error_free_exit(shell, ERR_SIGEMPTYSET);
		if (sigaction(SIGQUIT, &shell->sa_sigquit, NULL) == -1)
			error_free_exit(shell, ERR_SIGACTION);
	}
	else if (mode == PARENT_WITH_CHILD)
	{
		shell->sa_sigint.sa_handler = SIG_IGN;
		sigaction(SIGINT, &shell->sa_sigint, NULL);
	}
	else if (mode == CHILD)
	{
		shell->sa_sigint.sa_handler = child_sigint_handler;
		sigaction(SIGINT, &shell->sa_sigint, NULL);
		shell->sa_sigquit.sa_handler = child_sigquit_handler;
		sigaction(SIGQUIT, &shell->sa_sigquit, NULL);
	}
	else if (mode == HEREDOC_CHILD)
	{
		shell->sa_sigquit.sa_handler = SIG_IGN;
		shell->sa_sigquit.sa_flags = 0;
		if (sigemptyset(&shell->sa_sigquit.sa_mask) == -1)
			error_free_exit(shell, ERR_SIGEMPTYSET);
		if (sigaction(SIGQUIT, &shell->sa_sigquit, NULL) == -1)
			error_free_exit(shell, ERR_SIGACTION);
	}
}
