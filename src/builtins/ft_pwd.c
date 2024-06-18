/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msimic <msimic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:22:55 by msimic            #+#    #+#             */
/*   Updated: 2024/05/31 16:55:13 by msimic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int ft_pwd(t_minishell *shell)
{
    char *current_pwd;

    current_pwd = getcwd(NULL, 0);
    if (current_pwd == NULL)
        error_free_all(shell, ERR_GETCWD, NULL, NULL);
    else
        printf("%s\n", current_pwd);
    free_and_reset_ptr((void **)&current_pwd);
    return (0);
}