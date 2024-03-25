/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilin <lilin@student.42vienna.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 17:12:20 by lilin             #+#    #+#             */
/*   Updated: 2023/12/12 18:51:03 by lilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *s, char sep)
{
	int	i;
	int	words;

	i = 0;
	words = 0;
	while (s[i] && s[i] == sep)
		i++;
	while (s[i])
	{
		if (s[i] != sep && (s[i + 1] == sep || !s[i + 1]))
			words++;
		i++;
	}
	return (words);
}

static char	*put_word(char const *s, int start, int len)
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
		printf("i: %d word: %d freed %s\n", i, word, array[i]);
		free(array[i]);
		i++;
	}
	free(array);
	array = NULL;
}

static int	put_words(char const *s, char sep, char **array)
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
			while (s[i] && s[i] != sep)
				i++;
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

char	**ft_split(char const *s, char c)
{
	int		words;
	char	**array;

	if (!s)
		return (NULL);
	words = count_words(s, c);
	array = NULL;
	array = malloc((words + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	array[words] = NULL;
	if (put_words(s, c, array) == -1)
		return (NULL);
	return (array);
}
