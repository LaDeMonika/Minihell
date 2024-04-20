/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 15:09:56 by msimic            #+#    #+#             */
/*   Updated: 2024/04/20 16:54:04 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int ft_strcmp(char *s1, char *s2)
{
    int i;

    i = 0;
    while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
        i++;
    return (s1[i] - s2[i]);
}
/* 
static void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

static void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	while (*s)
	{
		ft_putchar_fd(*s, fd);
		s++;
	}
}
*/

static void ft_putstr_no_newline(char *str, int flag)
{
    while (*str)
    {
        if (*str == '\n')
            str++;
        write(1, str, 1);
        str++;
    }
    if (flag == 0)
        write(1, "\n", 1);
}

int ft_echo(t_minishell *shell)
{
    int i = 1;
    int new_line_flag = 0;

    while (shell->input_array[i])
    {
        if (ft_strcmp(shell->input_array[i], "echo") == 0)
        {
            if (ft_strcmp(shell->input_array[i + 1], "-n") == 0)
            {
                new_line_flag = 1;
                i++;
            }
            i++;
        }
        ft_putstr_no_newline(shell->input_array[i], new_line_flag);
        if (shell->input_array[i + 1])
            ft_putstr_fd(" ", 1); // Print a space if there is another argument
        i++;
    }
    return 0;
}
/* 
int ft_is_builtin(t_minishell *shell)
{
    int status;
    int i = 0;

    status = 1;
    while (shell->input_array[i])
    {
        if (ft_strcmp(shell->input_array[i], "echo") == 0)
        {
            status = ft_echo(shell);
        }
        i++;
    }
    if (status == 0)
        shell->exit_status = 0;
    return (status);
}

int main(int ac, char **av)
{
    t_minishell shell;

    if (ac < 2)
    {
        printf("Error: Not enough arguments\n");
        return (0);
    }
    shell.input_array = av;
    ft_is_builtin(&shell);
    return (0);
}
*/