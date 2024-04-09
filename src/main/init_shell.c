/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 14:26:33 by msimic            #+#    #+#             */
/*   Updated: 2024/04/09 10:11:46 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	init_shell_struct(t_minishell *shell, char **envp)
{
    shell->current_dir = getcwd(NULL, 0);
    shell->envp = envp;
    shell->last_exit_status = 0;
	shell->pid = getpid();
    shell->user = getenv("USER");
	shell->prompt = ft_strjoin(shell->user, "@");
	shell->prompt = ft_strjoin("fake_", shell->prompt);
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