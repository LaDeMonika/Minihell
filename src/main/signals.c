#include "../../inc/minishell.h"

void	child_sigint_handler(int sig)
{
	(void)sig;
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
		if (WCOREDUMP(shell->status))
		{
			if (remaining_children == 0)
				write(2, "Quit (core dumped)\n", 19);
		}
		else if (remaining_children < 2)
			write(2, "\n", 1);
	}
}
/*
with sigemptyset reset signals that can block the signal handling
with sa_flags determine no special signal handling*/
void	set_signals(t_minishell *shell, int mode)
{
	if (sigemptyset(&shell->sa_sigint.sa_mask) == -1)
		error_free_exit(shell, ERR_SIGEMPTYSET);
	shell->sa_sigint.sa_flags = 0;
	if (sigemptyset(&shell->sa_sigquit.sa_mask) == -1)
		error_free_exit(shell, ERR_SIGEMPTYSET);
	shell->sa_sigquit.sa_flags = 0;
	if (mode == PARENT_WITHOUT_CHILD)
		shell->sa_sigint.sa_handler = parent_sigint_handler;
	if (mode == PARENT_WITH_CHILD)
		shell->sa_sigint.sa_handler = SIG_IGN;
	if (mode == PARENT_WITHOUT_CHILD || mode == PARENT_WITH_CHILD
		|| mode == HEREDOC_CHILD)
		shell->sa_sigquit.sa_handler = SIG_IGN;
	if (mode == HEREDOC_CHILD)
		shell->sa_sigint.sa_handler = child_sigint_handler;
	if (mode == CHILD)
	{
		shell->sa_sigint.sa_handler = child_sigint_handler;
		shell->sa_sigquit.sa_handler = child_sigquit_handler;
	}
	if (sigaction(SIGINT, &shell->sa_sigint, NULL) == -1)
		error_free_exit(shell, ERR_SIGACTION);
	if (sigaction(SIGQUIT, &shell->sa_sigquit, NULL) == -1)
		error_free_exit(shell, ERR_SIGACTION);
}
