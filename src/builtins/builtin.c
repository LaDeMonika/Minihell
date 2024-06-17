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

/* bool handle_unset(t_minishell *shell, int *custom_errno)
{
    int i;
    int exit_status;

    i = 1;
    while (shell->command_array[i])
    {
        exit_status = ft_unset(shell, shell->command_array[i], custom_errno);
        i++;
    }
    return (exit_status);
} */

/* int handle_export(t_minishell *shell, int *custom_errno)
{
    int i;
    int exit_status;

    if (!shell->command_array[1])
        return (export_no_args(shell));
    i = 1;
    while (shell->command_array[i])
    {
        exit_status = ft_export(shell, shell->command_array[i], custom_errno);
        i++;
    }
    return (exit_status);
} */
int handle_builtin_for_each_arg(t_minishell *shell, int *custom_errno)
{
    int i;
    int exit_status;

    i = 1;
    while (shell->command_array[i])
    {
        if (shell->builtin == B_EXPORT)
            exit_status = ft_export(shell, shell->command_array[i], custom_errno);
        else if (shell->builtin == B_UNSET)
            exit_status = ft_unset(shell, shell->command_array[i], custom_errno);
        i++;
    }
    return (exit_status);
}

int handle_builtin(t_minishell *shell, int *custom_errno)
{
    if (shell->builtin == B_CD)
        return (ft_cd(shell, custom_errno));
    else if (shell->builtin == B_ECHO)
        return (ft_echo(shell));
    else if (shell->builtin == B_ENV)
        return (ft_env(shell, custom_errno));
     else if (shell->builtin == B_EXIT)
        return (ft_exit(shell, custom_errno));
    else if (shell->builtin == B_EXPORT)
    {
        if (!shell->command_array[1])
            return (export_no_args(shell));
        return (handle_builtin_for_each_arg(shell, custom_errno));
    }
    else if (shell->builtin == B_PWD)
        return (ft_pwd(shell));
    else if (shell->builtin == B_UNSET)
         return (handle_builtin_for_each_arg(shell, custom_errno));
    return (0);
}

int is_builtin(char *token)
{

    if (ft_strncmp(token, "cd",	3) == 0)
        return (B_CD);
    else if (ft_strcmp(token, "echo") == 0)
        return (B_ECHO);
    else if (ft_strcmp(token, "env") == 0)
        return (B_ENV);
	else if (ft_strcmp(token, "exit") == 0)
        return (B_EXIT);
    else if (ft_strcmp(token, "export") == 0)
        return (B_EXPORT);
    else if (ft_strcmp(token, "pwd") == 0)
        return (B_PWD);
	else if (ft_strcmp(token, "unset") == 0)
        return (B_UNSET);
    else
        return (NOT_BUILTIN);
}