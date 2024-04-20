/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_msg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 12:57:17 by msimic            #+#    #+#             */
/*   Updated: 2024/04/20 16:58:30 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void    ft_puterror(const char *fault, const char *msg)
{
    if (fault && ft_strlen(fault))
        printf("Error: %s\n", fault);
    if (msg && ft_strlen(msg))
        printf("Expected: %s\n", msg);
    printf("\n");
}

void    ft_error_msg(char err)
{
    if (err == ERR_MALLOC)
        ft_puterror("Memory allocation failed", "");
    else if (err == ERR_TOO_MANY_ARGS)
        ft_puterror("Too many arguments", "Please try again");
    else if (err == ERR_TOO_FEW_ARGS)
        ft_puterror("Too few arguments", "Please try again");
    else if (err == ERR_INVALID_ARG)
        ft_puterror("Invalid argument", "Please try again");
    else if (err == ERR_PATH_NOT_FOUND)
        ft_puterror("Path not found", "");
}

void    free_all(t_minishell *shell)
{
    if (shell->prompt)
        free(shell->prompt);
}

void	error_free_exit(t_minishell *shell, char err)
{
    if (err == ERR_SIGEMPTYSET)
        perror("sigemptyset");
    else if (err == ERR_SIGACTION)
        perror("sigaction");
    else if (err == ERR_OPEN)
        perror("open");
    else if (err == ERR_READ)
        perror("read");
    else if (err == ERR_CLOSE)
        perror("close");
    else if (err == ERR_MALLOC)
        perror("malloc");
    free_all(shell);
	exit(EXIT_FAILURE);
}