/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_write.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:27:18 by lilin             #+#    #+#             */
/*   Updated: 2024/06/24 18:28:14 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*first determine new string length and then create new string without any
metaquotes*/
char	*write_literal_chars(char *str, char *new_str)
{
	char	metaquote;
	int		i;
	int		j;

	metaquote = '\0';
	i = 0;
	j = 0;
	while (str && str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			if (!(metaquote))
				metaquote = str[i];
			else if (metaquote == str[i])
				metaquote = '\0';
			else if (str[i] != metaquote)
				copy_char_and_increment_new_str(new_str, str[i], &j);
		}
		else
			copy_char_and_increment_new_str(new_str, str[i], &j);
		i++;
	}
	return (new_str);
}

char	*remove_metaquotes(t_minishell *shell, char *str)
{
	int		new_len;
	char	*new_str;

	new_len = count_literal_chars(str);
	new_str = try_malloc(shell, sizeof(char) * (new_len + 1));
	new_str[new_len] = '\0';
	new_str = write_literal_chars(str, new_str);
	free_and_reset_ptr((void **)&str);
	return (new_str);
}
