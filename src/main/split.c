#include "../../inc/minishell.h"

/*
if sep is pipe start word count at one and at one for every pipe
if sep is space,
	then start word count at zero and only add for space before other character or null-terminator*/
static int	count_words(char *s, char sep)
{
	int		i;
	int		words;
	char	quote_type;

	i = 0;
	if (sep == '|')
		words = 1;
	else
		words = 0;
	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
			i = skip_between_quotes(s, i, s[i]);
		if ((sep == '|' && s[i] == sep)
		|| (sep == ' ' && s[i] != sep && (s[i + 1] == sep || !s[i + 1] )))
			words++;
		i++;
	}
	return (words);
}
/*if sep is | and word is empty than substitute empty word with |*/
static char	*put_word(char *s, int start, int len, char sep)
{
	char	*word;

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


/*
put word until separator, and skip quotes
if sep is | and a word is missing, then replace word with | */
static int	put_words(char *s, char sep, char **array, int words)
{
	int	i;
	int	start;
	int	word;

	i = 0;
	start = 0;
	word = 0;
	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
			i = skip_between_quotes(s, i, s[i]);
		if (!s[i]
		|| (sep == '|' && (s[i] == sep || !s[i + 1]))
		|| (sep == ' ' && s[i] != sep && (s[i + 1] == sep || !s[i + 1])))
		{
			/* printf("at index %d will make a word with start: %d and len:
				%d\n", i, start, i - start); */
			if (sep == '|' && s[i] == sep)
				array[word] = put_word(s, start, i - start, sep);
			else if (s[i] != sep)
				array[word] = put_word(s, start, i - start + 1, sep);
			if (!array[word])
				return (free_array(array, word), -1);
			word++;
			start = i + 1;
		}
		i++;
	}
	if (word < words && sep == '|')
	{
		array[word] = malloc(sizeof(char) * 2);
		array[word] = "|";
	}
	return (0);
}

char	**split_skip_quotes(t_minishell *shell, char *s, char c)
{
	int		words;
	char	**array;
	int		i;

	if (!s || !s[0])
		return (NULL);
	words = count_words(s, c);
	array = NULL;
	array = malloc((words + 1) * sizeof(char *));
	if (!array)
		return (free_exit(shell, ERR_MALLOC), NULL);
	array[words] = NULL;
	if (put_words(s, c, array, words) == -1)
		return (NULL);
	i = 0;
	return (array);
}
/* int	main(void)
{
	char **array = ft_split("echo \"ls -l | grep m\" | grep i", '|');

	int i = 0;
	while (array[i])
	{
		printf("word at index %d: %s\n", i, array[i]);
		i++;
	}
} */