/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 11:01:14 by msimic            #+#    #+#             */
/*   Updated: 2024/05/31 17:05:30 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void    append_path(t_minishell *shell)
{
	char	*pwd;
	char	new_path[PATH_MAX];
	char	*home;

	pwd = getcwd(NULL, 0);
	home = getenv("HOME");
	if (ft_strnstr(pwd, home, ft_strlen(home)))
	{
		ft_strlcpy(new_path, pwd + ft_strlen(home), ft_strlen(pwd) - ft_strlen(home) + 1);
        shell->prompt = append_suffix(shell, shell->prompt, "~");
		shell->prompt = append_suffix(shell, shell->prompt, new_path);
	}
	else
		shell->prompt = append_suffix(shell, shell->prompt, pwd);
	free_and_reset_ptr((void **)&pwd);
}

void    append_hostname(t_minishell *shell)
{
    int		hostname_file;
	char	hostname[254];
	char	*hostname_remainder;

	hostname_file = try_open(shell, "/etc/hostname", READ);
	if (read(hostname_file, hostname, 254) == -1)
		error_free_all(shell, ERR_READ, NULL, NULL);
	if (close(hostname_file) == -1)
		error_free_all(shell, ERR_CLOSE, NULL, NULL);
	hostname_remainder = ft_strchr(hostname, '.');
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
