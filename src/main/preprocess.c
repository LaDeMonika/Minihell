/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 18:38:26 by lilin             #+#    #+#             */
/*   Updated: 2024/06/24 18:38:27 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*extract_and_append(t_minishell *shell, char *base, int len)
{
	shell->suffix = ft_substr(shell, base, 0, len);
	shell->temp_str = append(shell, shell->temp_str, shell->suffix, BOTH);
	return (shell->temp_str);
}

bool	found_quote(t_minishell *shell, char *s, int *i)
{
	if (s[*i] == '"' && !shell->metaquote)
		shell->metaquote = s[*i];
	else if (s[*i] == shell->metaquote)
		shell->metaquote = '\0';
	else if (s[*i] == '\'' && !shell->metaquote)
		*i = skip_between_metaquotes(s, *i, '\'');
	else
		return (false);
	return (true);
}

char	*expand_env_variables(t_minishell *shell, char *s)
{
	int	i;
	int	start;

	i = 0;
	start = 0;
	shell->metaquote = '\0';
	while (s[i])
	{
		if (!found_quote(shell, s, &i) && (s[i] == '$' || (s[i] == '~'
					&& !shell->metaquote && (is_space(s[i + 1]) || !s[i + 1]))))
		{
			shell->temp_str = extract_and_append(shell, s + start, i - start);
			shell->env_value = get_env_value(shell, s, &start, &i);
			shell->temp_str = append(shell, shell->temp_str, shell->env_value,
					BOTH);
			shell->env_value = NULL;
		}
		if (s[i])
			i++;
	}
	if (i != start)
		shell->temp_str = extract_and_append(shell, s + start, i - start);
	free_and_reset_ptr((void **)&s);
	return (shell->temp_str);
}

char	*append_heredoc_on_missing_quote(t_minishell *shell, char *base)
{
	int		i;
	char	quote[2];

	i = 0;
	while (base[i])
	{
		if (base[i] == '"' || base[i] == '\'')
		{
			shell->metaquote = base[i];
			i = skip_between_metaquotes(base, i, shell->metaquote);
			if (!base[i])
			{
				quote[0] = shell->metaquote;
				quote[1] = '\0';
				shell->temp_str = append(shell, base, quote, BASE);
				shell->temp_str = append(shell, shell->temp_str, "<<", BASE);
				shell->temp_str = append(shell, shell->temp_str, quote, BASE);
				free_and_reset_ptr((void **)&base);
				return (shell->temp_str);
			}
		}
		i++;
	}
	return (base);
}
