/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:21:57 by msimic            #+#    #+#             */
/*   Updated: 2024/05/24 18:36:09 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
function for the env command in bash
prints the linked list of env variables
#Next:
- protection
#Cases:
- env
- env -i
- env -i command
- env -i command arg1 arg2
- env -i command arg1 arg2 > file
- env -i command arg1 arg2 < file
...
*/

/* int ft_add_env(char *name, char *value)
{

} */

int ft_env(t_minishell *shell, char **command_array)
{
    // probably will need to implement something for redirections, not sure yet
    int i;

    i = 0;
    if (command_array[1])
    {
        if (strchr(command_array[1], '='))
        {
            ft_export(shell, command_array[1]);
            if (command_array[2])
            {
                shell->command_array = command_array + 2;
                execute_command_array(shell, command_array + 2);
            }
            else
            {

            }

        }
        else
        {
            shell->command_array = command_array + 1;
            execute_command_array(shell, command_array + 1);
        }

    }
    else
    {
        while (shell->envp[i])
        {
            printf("%s\n", shell->envp[i]);
            i++;
        }
    }
    return (0);
}