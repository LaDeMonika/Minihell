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
    char		cwd[PATH_MAX];

    if (!getcwd(cwd, PATH_MAX))
        error_free_all(shell, ERR_GETCWD, NULL, NULL);
    printf("%s\n", cwd);
    return (0);
}