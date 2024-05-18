#include "../../inc/minishell.h"
#include <stdbool.h>

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


/*
with sigemptyset reset signals that can block the signal handling
with sa_flags determine no special signal handling*/
void	set_signals(t_minishell *shell, int mode)
{
	bool	is_child;

	is_child = false;
	if (mode == CHILD || mode == HEREDOC_CHILD)
		is_child = true;
	if (sigemptyset(&shell->sa_sigint.sa_mask) == -1 || sigemptyset(&shell->sa_sigquit.sa_mask) == -1)
		error_free_all(shell, ERR_SIGEMPTYSET, NULL, NULL);
	shell->sa_sigint.sa_flags = 0;
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
	if (sigaction(SIGINT, &shell->sa_sigint, NULL) == -1 || sigaction(SIGQUIT, &shell->sa_sigquit, NULL) == -1)
		error_free_all(shell, ERR_SIGACTION, NULL, NULL);
}