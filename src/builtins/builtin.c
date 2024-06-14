#include "../../inc/minishell.h"

int ft_strcmp(char *s1, char *s2)
{
    int i;

    i = 0;
    if (!s1 && !s2)
        return (0);
    if (!s1 || !s2)
        return (1);
    while (s1[i] == s2[i] && s1[i] && s2[i])
        i++;
    if (s1[i] != s2[i])
        return (1);
    return (s1[i] - s2[i]);
}

char ft_strcmp_btin(char *s1, char *s2)
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
int export_no_args(t_minishell  *shell)
{
    int i;
    char  *key;
    char *value;

    i = 0;
    while (shell->envp[i])
    {
        key = ft_substr(shell, shell->envp[i], 0,  index_of_first_occurence(shell->envp[i], '='));
        value = ft_substr(shell, shell->envp[i], index_of_first_occurence(shell->envp[i], '=') + 1, ft_strlen(strchr(shell->envp[i], '=') - 1));
        printf("declare -x %s=\"%s\"\n", key, value);
        free_and_reset_ptr((void **)&key);
        free_and_reset_ptr((void **)&value);
        i++;
    }
    return (0);
}

bool handle_unset(t_minishell *shell, char **command_array, int *status, int *custom_errno)
{
    int i;

    i = 1;
    while (command_array[i])
    {
        *status = ft_unset(shell, command_array[i], custom_errno);
        i++;
    }
    return (true);
}

bool handle_export(t_minishell *shell, char **command_array, int *status, int *custom_errno)
{
    int i;

    if (!command_array[1])
    {
        *status = export_no_args(shell);
        return (true);
    }
    i = 1;
    while (command_array[i])
    {
        *status = ft_export(shell, command_array[i], custom_errno);
        i++;
    }
    return (true);
}

bool ft_is_builtin(t_minishell *shell, char **command_array, int *status, int *custom_errno)
{
    if (ft_strcmp_btin(command_array[0], "echo") == 0)
        *status = ft_echo(command_array + 1);
    else if (ft_strcmp_btin(command_array[0], "cd") == 0)
        *status = ft_cd(shell, command_array, custom_errno);
    else if (ft_strcmp_btin(command_array[0], "pwd") == 0)
        *status = ft_pwd(shell);
    else if (ft_strcmp_btin(shell->command_array[0], "env") == 0)
        *status = ft_env(shell, custom_errno);
    else if (ft_strcmp_btin(shell->command_array[0], "unset") == 0)
         return(handle_unset(shell, command_array, status, custom_errno));
    else if (ft_strcmp_btin(shell->command_array[0], "export") == 0)
        return (handle_export(shell, command_array, status, custom_errno));
    else if (ft_strcmp_btin(shell->command_array[0], "exit") == 0)
        *status = ft_exit(shell, shell->command_array, custom_errno);
    else
        return (false);
    return (true);
}
