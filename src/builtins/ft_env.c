/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:21:57 by msimic            #+#    #+#             */
/*   Updated: 2024/05/17 13:31:08 by msimic           ###   ########.fr       */
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
    int i = 0;
    int j = 0;
    
    if (ft_strcmp(command_array[i], "env") == 0)
    {
        // probably will need to implement something for redirections, not sure yet
        if (command_array[j + 1] == NULL)
        {
            while (shell->envp[j])
	        {
		        printf("%s\n", shell->envp[j]);
		        j++;
	        }
        }
    }
    return (0);
}