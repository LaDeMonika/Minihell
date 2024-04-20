/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 11:01:14 by msimic            #+#    #+#             */
/*   Updated: 2024/04/20 17:27:32 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <linux/limits.h>

char	*append_to_prompt(t_minishell *shell, char *s)
{
	char	*tmp_prompt;

	tmp_prompt = shell->prompt;
	shell->prompt = ft_strjoin(shell->prompt, s);
	if (!shell->prompt)
		error_free_exit(shell, ERR_MALLOC);
	if (tmp_prompt)
		free(tmp_prompt);
	return (shell->prompt);
}

/*
if current directory includes the home path, then replace home by ~
*/
void    append_path(t_minishell *shell)
{
	char	*pwd;
	char	new_path[PATH_MAX];
	char	*home;

	pwd = getenv("PWD");
	home = getenv("HOME");
	if (ft_strnstr(pwd, home, ft_strlen(home)))
	{
		ft_strlcpy(new_path, pwd + ft_strlen(home), ft_strlen(pwd)
			- ft_strlen(home) + 1);
        append_to_prompt(shell, "~");
		append_to_prompt(shell, new_path);
	}
	else
		append_to_prompt(shell, pwd);
}

void    append_hostname(t_minishell *shell)
{
    int		hostname_file;
	char	hostname[254];
	char	*hostname_remainder;

    hostname_file = open("/etc/hostname", O_RDONLY);
	if (hostname_file == -1)
		error_free_exit(shell, ERR_OPEN);
	if (read(hostname_file, hostname, 254) == -1)
		error_free_exit(shell, ERR_READ);
	if (close(hostname_file) == -1)
		error_free_exit(shell, ERR_CLOSE);
	hostname_remainder = strchr(hostname, '.');
	if (hostname_remainder)
		hostname[hostname_remainder - hostname] = '\0';
	append_to_prompt(shell, hostname);
}

void	build_prompt(t_minishell *shell)
{
    append_to_prompt(shell, getenv("USER"));
	append_to_prompt(shell, "@@");
    append_hostname(shell);
	append_to_prompt(shell, ":");
	append_path(shell);
    append_to_prompt(shell, "$ ");
}
