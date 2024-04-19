/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 14:26:33 by msimic            #+#    #+#             */
/*   Updated: 2024/04/19 12:40:43 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	init_shell_struct(t_minishell *shell, char **envp)
{
    shell->current_dir = getcwd(NULL, 0);
    shell->envp = envp;
    shell->last_exit_status = 0;
	shell->pid = NULL;
    shell->user = getenv("USER");
	shell->prompt = ft_strjoin(shell->user, "@");
	shell->prompt = ft_strjoin("fake_", shell->prompt);
	shell->pipes_total = 0;

	//shell->fd_hostname = open("/etc/hostname", O_RDONLY);
	//shell->bytes_read = read(fd_hostname, hostname_buffer, 254);
	//close(fd_hostname);
	//hostname_len = strchr(hostname_buffer, '.') - hostname_buffer;
	//hostname_buffer[hostname_len] = '\0';
	// shell->prompt = ft_strjoin(prompt, hostname_buffer);
	// shell->prompt = ft_strjoin(prompt, ":");
	// shell->path_temp = getenv("PWD");
	// shell->home = getenv("HOME");
}
