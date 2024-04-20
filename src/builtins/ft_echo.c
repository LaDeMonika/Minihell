/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 15:09:56 by msimic            #+#    #+#             */
/*   Updated: 2024/04/20 12:16:47 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* static int ft_strcmp(char *s1, char *s2)
{
    int i;

    i = 0;
    while (s1[i] && s2[i])
    {
        if (s1[i] != s2[i])
            return (0);
        i++;
    }
    if (s1[i] != s2[i])
        return (0);
    return (1);
}

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
} */

/*
ft_echo is a function that will print the arguments to the standard output.
*/
int ft_echo(t_minishell *shell)
{
    int i;
    
    i = 1;
    while (shell->input_array[i])
    {
        ft_putstr_fd(shell->input_array[i], 1); // Print the argument
        if (shell->input_array[i + 1])
            ft_putstr_fd(" ", 1); // Print a space if there is another argument
        i++;
    }
    ft_putstr_fd("\n", 1);
    return (1);
}

/* int ft_is_builtin(t_minishell *shell)
{
    int status;

    status = 1; // 1 = builtin, 0 = not builtin
    if (ft_strcmp(shell->input_array[0], "echo") == 5) // 5 = length of "echo"
        status = ft_echo(shell);
    if (status == 0)
        shell->exit_status = 0;
    return (status);
}

int main(int ac, char **av)
{
    t_minishell shell;

    shell.input_array = av;
    ft_is_builtin(&shell);
    return (0);
} */