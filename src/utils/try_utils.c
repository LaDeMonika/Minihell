/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   try_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:29:40 by lilin             #+#    #+#             */
/*   Updated: 2024/06/24 18:29:41 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	*try_malloc(t_minishell *shell, int size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		error_free_all(shell, ERR_MALLOC, NULL, NULL);
	return (ptr);
}

void	try_pipe(t_minishell *shell, int fd[2])
{
	if (pipe(fd) == -1)
		error_free_all(shell, ERR_PIPE, NULL, NULL);
}

int	try_fork(t_minishell *shell)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		return (error_free_all(shell, ERR_FORK, NULL, NULL), 1);
	return (pid);
}

void	try_getcwd(t_minishell *shell)
{
	if (!getcwd(shell->cwd, PATH_MAX))
		error_free_all(shell, ERR_GETCWD, NULL, NULL);
}
