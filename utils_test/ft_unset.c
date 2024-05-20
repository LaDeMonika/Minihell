/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:23:13 by msimic            #+#    #+#             */
/*   Updated: 2024/05/20 13:56:23 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_unset.h"

int ft_strlen(char *str)
{
    int i = 0;

    while (str[i])
        i++;    
    return (i);
}


int ft_strcmp(char *s1, char *s2)
{
    int i;

    i = 0;
    while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
        i++;
    return (s1[i] - s2[i]);
}

void	init_shell_struct(t_minishell *shell, char **envp)
{

	shell->envp = envp;
}

/*
The `unset` command is used in Unix-like operating systems to unset or
remove environment variables and shell functions. 
*/
int ft_unset(t_minishell *shell, char **command_array)
{
    int i = 0;
    
    while (shell->envp[i])
    {
        if (ft_strcmp(shell->envp[i], command_array[1]) == 0)
        {
            // remove the variable from the envp list
            // free(shell->envp[i]);
            // shell->envp[i] = NULL;
            // shift the rest of the list
            printf("found the variable to unset\n");
            return (0);
        }
        i++;
    }
    //so i should iterate through the env list and remove the variables that are in the command_array
    //if it's iterating ([if] in [while] loop) and char matching one by one and check just till '='; then delete that variable
    //else go to next variable and check if it is that variable
    return (0);
}

int main(int argc, char **argv, char **envp)
{
    t_minishell *shell = (t_minishell *)malloc(sizeof(t_minishell));
    if (!shell)
        return (1);
    int i = 0;
    
    //Test builtin ft_unset command but other commands should be available
    init_shell_struct(shell, envp);
    while (shell->envp[i])
    {
        printf("%s\n", shell->envp[i]);
        //check if the variable is in the envp list and if it comes to = then remove it
        printf("------>");
        ft_unset(shell, argv);
        i++;
    }
    free(shell);
}