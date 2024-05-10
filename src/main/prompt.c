/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 11:01:14 by msimic            #+#    #+#             */
/*   Updated: 2024/05/10 14:25:16 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <linux/limits.h>

char	*append_suffix(t_minishell *shell, char *base, char *suffix)
{
	char	*new_str;

	new_str = base;
	new_str = ft_strjoin(base, suffix);
	if (!new_str)
		free_exit(shell, ERR_MALLOC);
	free_and_reset_ptr((void **)&base);
	return (new_str);
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
        shell->prompt = append_suffix(shell, shell->prompt, "~");
		shell->prompt = append_suffix(shell, shell->prompt, new_path);
	}
	else
		shell->prompt = append_suffix(shell, shell->prompt, pwd);
}

void    append_hostname(t_minishell *shell)
{
    int		hostname_file;
	char	hostname[254];
	char	*hostname_remainder;

    hostname_file = open("/etc/hostname", O_RDONLY);
	if (hostname_file == -1)
		free_exit(shell, ERR_OPEN);
	if (read(hostname_file, hostname, 254) == -1)
		free_exit(shell, ERR_READ);
	if (close(hostname_file) == -1)
		free_exit(shell, ERR_CLOSE);
	hostname_remainder = strchr(hostname, '.');
	if (hostname_remainder)
		hostname[hostname_remainder - hostname] = '\0';
	shell->prompt = append_suffix(shell, shell->prompt, hostname);
}

void	build_prompt(t_minishell *shell)
{
    shell->prompt = append_suffix(shell, shell->prompt, getenv("USER"));
	shell->prompt = append_suffix(shell, shell->prompt, "@@");
    append_hostname(shell);
	shell->prompt = append_suffix(shell, shell->prompt, ":");
	append_path(shell);
    shell->prompt = append_suffix(shell, shell->prompt, "$ ");
}
