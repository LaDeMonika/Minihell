#include "../../inc/minishell.h"

int ft_strcmp(char *s1, char *s2)
{
    int i;

    i = 0;
    if (!s1 && !s2)
        return (0);
    if (!s1 || !s2)
        return (1);
    while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
        i++;
    if (s1[i] != s2[i])
        return (1);
    return (s1[i] - s2[i]);
}

static char ft_strcmp_btin(char *s1, char *s2)
{
    int i;

    i = 0;
    while (s1[i] && s2[i])
    {
        if (s1[i] != s2[i])
            return (1);
        i++;
    }
    if (s1[i] != s2[i])
        return (1);
    return (0);
}

/*
ft_is_builtin is a function that will check if the command is a builtin command.
If the command is a builtin command, it will execute the builtin command.
If the command is not a builtin command, it will return 0.
*/
int ft_is_builtin(t_minishell *shell, char **command_array)
{
    int status;

    status = 1;
    if (ft_strcmp_btin(command_array[0], "echo") == 0)
        status = ft_echo(command_array);
    else if (ft_strcmp_btin(command_array[0], "cd") == 0)
        status = ft_cd(command_array);
    else if (ft_strcmp_btin(command_array[0], "pwd") == 0)
        status = ft_pwd(command_array);
    else if (ft_strcmp_btin(shell->command_array[0], "env") == 0)
        status = ft_env(shell, command_array);
    else if (ft_strcmp_btin(shell->command_array[0], "unset") == 0)
        status = ft_unset(shell, command_array); //wip
    /* else if (ft_strcmp_btin(shell->command_array[0], "export") == 0)
        status = ft_export(shell, command_array);
    else if (ft_strcmp_btin(shell->command_array[0], "exit") == 0)
        status = ft_exit(command_array); */
    else
        status = 2;
    //printf("\nft_is_buitin [0 = builtin, 2 = not builtin]: %d\n\n", status);
    return (status);
}