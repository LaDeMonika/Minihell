/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_00.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 17:12:20 by lilin             #+#    #+#             */
/*   Updated: 2024/04/18 20:02:03 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../libft/libft.h"
#include <bits/types/struct_itimerspec.h>
#include <stdlib.h>

static char	*add_heredoc(char *s, char quote_marker)
{
	char	*new_s;
	char	end_quote[2];

	end_quote[0] = quote_marker;
	end_quote[1] = '\0';
	new_s = ft_strjoin((const char*)s, "<<");
	new_s = ft_strjoin(new_s, end_quote);
	return (new_s);
}

static char	find_quote(char *s, int *start, int *end, int *markers)
{
	int	i;
	char	quote_marker;
	int	len;

	*start = -1;
	*end = -1;
	len = ft_strlen(s);
	*markers = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
		{
			quote_marker = s[i];
			*start = i;
			*markers += 1;
			while(s[len - 1])
			{
				if (s[len - 1] == quote_marker && len - 1 != *start)
				{
					if (*markers == 1)
						*end = len - 1;
					*markers += 1;
				}
				len--;
			}
			return (quote_marker);
		}
		i++;
	}
	return('\0');
}

static int	count_words(char *s, char sep, char quote_marker, int start_quote, int end_quote)
{
	int	i;
	int	words;

	i = 0;
	words = 0;
	while (s[i] && s[i] == sep)
		i++;
	while (s[i])
	{
		//only count outside of quotation marks
		if (quote_marker != '\0' && i == start_quote)
		{
			while (i < end_quote)
			i++;
		}
		if (s[i] != sep && (s[i + 1] == sep || !s[i + 1]))
			words++;
		i++;
	}
	return (words);
}

static char	*put_word(char *s, int start, int len)
{
	int		i;
	char	*word;

	i = 0;
	word = NULL;
	word = malloc((len + 1) * sizeof(char));
	if (!word)
		return (NULL);
	word[len] = '\0';
	while (i < len)
	{
		word[i] = s[start + i];
		i++;
	}
	return (word);
}

static void	free_array(char **array, int word)
{
	int	i;

	i = 0;
	while (i < word)
	{
		free(array[i]);
		i++;
	}
	free(array);
	array = NULL;
}

static int	put_words(char *s, char sep, char **array, char quote_marker, int start_quote, int end_quote)
{
	int	i;
	int	word;
	int	start;

	i = 0;
	word = 0;
	while (s[i])
	{
		if (s[i] != sep)
		{
			start = i;
			while (s[i])
			{
				if (quote_marker != '\0' && i == start_quote)
				{
					while (i < end_quote)
						i++;
				}
				if (s[i] != sep)
					i++;
				else
				 	break;
			}
			array[word] = put_word(s, start, i - start);
			if (!array[word])
			{
				free_array(array, word);
				return (-1);
			}
			word++;
		}
		else
			i++;
	}
	return (0);
}

char	*append_affix(char *old_s, int start, int len, char *new_s)
{
	char	*affix;

	affix = ft_substr(old_s, start, len - start);
	if (new_s)
		new_s = ft_strjoin(new_s, affix);
	else
		new_s = affix;
	return (new_s);
}

char *check_env_variables(t_minishell *shell, char *s)
{
	int	i;
	int	start;
	char	*env_key;
	char	*env_value;
	char	*new_s;

	i = 0;
	start = 0;
	new_s = NULL;
	while (s[i])
	{
		if (s[i] == '$')
		{
			new_s = append_affix(s, start, i, new_s);
			start = i + 1;
			while (s[i] && s[i] != ' ' && s[i] != '"')
					i++;
			if (s[start] == '?')
				env_value = ft_itoa(shell->last_exit_status);
			else
			{
				env_key = ft_substr(s, start, i - start);
				env_value = getenv(env_key);
			}
			if (env_value)
				new_s = ft_strjoin(new_s, env_value);
			else
			{
				new_s = ft_strjoin(new_s, "$");
				new_s = ft_strjoin(new_s, env_key);
			}

			start = i;

		}
		i++;
	}
	if (i != start)
		new_s = append_affix(s, start, i, new_s);
	return (new_s);
}

char	**ft_split_ignore_quotes(t_minishell *shell, char *s, char c)
{
	int		words;
	char	**array;
	char	quote_marker;
	int	start_quote;
	int	end_quote;
	int	markers;

	if (!s)
		return (NULL);
	quote_marker = find_quote(s, &start_quote, &end_quote, &markers);
	if (!quote_marker || quote_marker == '"')
		s = check_env_variables(shell, s);
	if (quote_marker != '\0' && markers % 2 != 0)
		s = add_heredoc(s, quote_marker);

	words = count_words(s, c, quote_marker, start_quote, end_quote);

	array = NULL;
	array = malloc((words + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	array[words] = NULL;
	if (put_words(s, c, array, quote_marker, start_quote, end_quote) == -1)
		return (NULL);
	return (array);
}
/* int	main()
{
	char **array = ft_split("echo \"ls -l | grep m\" | grep i", '|');

	int i = 0;
	while (array[i])
	{
		printf("%s\n", array[i]);
		free(array[i]);
		i++;
	}
	free(array);
} */