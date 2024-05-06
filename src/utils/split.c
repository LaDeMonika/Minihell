#include "../../inc/minishell.h"
#include "../../libft/libft.h"
#include <bits/types/struct_itimerspec.h>
#include <stdbool.h>
#include <stdlib.h>




/* static char	find_quote(char *s, int *start, int *end, int *markers)
{
	int		i;
	char	quote_marker;
	int		len;

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
			while (s[len - 1])
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
	return ('\0');
}
 */


/*s is guaranteed to have an even amount of outer quotes
if sep is pipe start word count at one and at one for every pipe
if sep is space, then start word count at zero and only add for space before other character or null-terminator*/
static int	count_words(char *s, char sep)
{
	int	i;
	int	words;
	char	quote_type;

	i = 0;
	if (sep == '|')
		words = 1;
	else
	 	words = 0;
	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
		{
			quote_type = s[i];
			i++;
			while (s[i] && s[i] != quote_type)
				i++;
		}
		else if ((sep == '|' && s[i] == sep)
		|| (sep == ' ' && s[i] != sep && (!s[i + 1] || s[i + 1] != sep)))
			words++;
		i++;
	}
	return (words);
}
/*if sep is | and word is empty than substitute empty word with |*/
static char	*put_word(char *s, int start, int len, char sep)
{
	/* int		i; */
	char	*word;

	/* i = 0;
	word = NULL;
	word = malloc((len + 1) * sizeof(char));
	if (!word)
		return (NULL);
	word[len] = '\0';
	while (i < len)
	{
		word[i] = s[start + i];
		i++;
	} */
	word = ft_substr(s, start, len);
	word = ft_strtrim(word, " ");
	if (sep == '|' && (!word || !(*word)))
	{
		free(word);
		word = malloc(sizeof(char) * 2);
		word = "|";
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

static int	put_words(char *s, char sep, char **array, int words)
{
	int	i;
	int	start;
	int	word;
	char	outer_quote;

	i = 0;
	start = 0;
	word = 0;
	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
		{
			outer_quote = s[i];
			i++;
			while (s[i] && s[i] != outer_quote)
				i++;
		}
		else if ((sep == '|' && (s[i] == sep || !s[i + 1]))
		|| (sep == ' ' && s[i] != sep && (!s[i + 1] || s[i + 1] != sep)))
		{
			printf("start: %d i: %d len: %d\n", start, i, i - start);
			array[word] = put_word(s, start, i - start, sep);
			if (!array[word])
				return (free_array(array, word), -1);
			word++;
			start = i + 1;
		}
		i++;
	}
	printf("word: %d words: %d\n", word, words);
	if (word < words && sep == '|')
	{
		array[word] = malloc(sizeof(char) * 2);
		array[word] = "|";
	}
	return (0);
}


/*
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
			new_s = append_substring(s, start, i, new_s);
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
		new_s = append_substring(s, start, i, new_s);
	return (new_s);
} */
/* static char	*add_heredoc(char *old_s, char quote_marker)
{
	(void)old_s;
	(void)quote_marker;
	return (old_s);
}
 */
char	**split_skip_quotes(t_minishell *shell, char *s, char c)
{
	int		words;
	char	**array;
	/* char	quote_marker;
	int		start_quote;
	int		end_quote;
	int		markers; */

	(void)shell;
	if (!s || !s[0])
		return (NULL);
/* 	quote_marker = find_quote(s, &start_quote, &end_quote, &markers);
	if (c == '|')
	{
		if (!quote_marker || quote_marker == '"')
			s = check_env_variables(shell, s);
		if (quote_marker != '\0' && markers % 2 != 0)
			s = add_heredoc(s, quote_marker);
	} */
	words = count_words(s, c);
	printf("amount of words: %d\n", words);
	array = NULL;
	array = malloc((words + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	array[words] = NULL;
	if (put_words(s, c, array, words) == -1)
		return (NULL);

	int i = 0;
	while (array[i])
	{
		printf("index: %d word: %s\n", i, array[i]);
		//free(array[i]);
		i++;
	}
	return (array);
}
/* int	main(void)
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