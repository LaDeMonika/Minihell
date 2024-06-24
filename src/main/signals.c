/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:30:26 by lilin             #+#    #+#             */
/*   Updated: 2024/06/24 18:30:27 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	set_signals(t_minishell *shell, int mode)
{
	if (sigemptyset(&shell->sa_sigint.sa_mask) == -1
		|| sigemptyset(&shell->sa_sigquit.sa_mask) == -1)
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
	if (mode == HEREDOC_CHILD || mode == CHILD)
		shell->sa_sigint.sa_handler = child_sigint_handler;
	if (mode == CHILD)
		shell->sa_sigquit.sa_handler = child_sigquit_handler;
	if (sigaction(SIGINT, &shell->sa_sigint, NULL) == -1 || sigaction(SIGQUIT,
			&shell->sa_sigquit, NULL) == -1)
		error_free_all(shell, ERR_SIGACTION, NULL, NULL);
}
