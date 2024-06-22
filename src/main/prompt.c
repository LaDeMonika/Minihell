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
	char	new_path[PATH_MAX];
	char	*home;

	try_getcwd(shell);
	home = getenv("HOME");
	if (ft_strnstr(shell->cwd, home, ft_strlen(home)))
	{
		ft_strlcpy(new_path, shell->cwd + ft_strlen(home), ft_strlen(shell->cwd) - ft_strlen(home) + 1);
        shell->prompt = append(shell, shell->prompt, "~", BASE);
		shell->prompt = append(shell, shell->prompt, new_path, BASE);
	}
	else
		shell->prompt = append(shell, shell->prompt, shell->cwd, BASE);
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
	shell->prompt = append(shell, shell->prompt, hostname, BASE);
}

void	build_prompt(t_minishell *shell)
{
    shell->prompt = append(shell, shell->prompt, getenv("USER"), BASE);
	shell->prompt = append(shell, shell->prompt, "@@", BASE);
    append_hostname(shell);
	shell->prompt = append(shell, shell->prompt, ":", BASE);
	append_path(shell);
    shell->prompt = append(shell, shell->prompt, "$ ", BASE);
}
