/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:22:55 by msimic            #+#    #+#             */
/*   Updated: 2024/05/24 16:17:43 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
pwd prints the current working directory
*/

int ft_pwd(t_minishell *shell)
{

    char *current;

    current = getcwd(NULL, 0);
    if (current == NULL)
        error_free_all(shell, ERR_GETCWD, NULL, NULL);
    else
        printf("%s\n", current);
    free_and_reset_ptr((void **)&current);
    return (0);
}