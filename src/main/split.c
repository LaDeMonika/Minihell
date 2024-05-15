#include "../../inc/minishell.h"

/*
if sep is pipe start word count at one and at one for every pipe
if sep is space,
	then start word count at zero and only add for space before other character or null-terminator*/
static int	count_words(char *s, char sep)
{
	int	i;
	int	words;

	i = 0;
	if (sep == '|')
		words = 1;
	else
		words = 0;
	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
			i = skip_between_quotes(s, i, s[i]);
		if ((sep == '|' && s[i] == sep) || (sep == ' ' && s[i] != sep && (s[i
					+ 1] == ' ' || !s[i + 1])))
			words++;
		i++;
	}
	return (words);
}
/*
if sep i ' ' then increase len by one so that it also puts the current char*/
static char	*replace_empty_word(t_minishell *shell, char *word, char sep)
{
	char	*temp;

	temp = word;
	word = ft_strtrim(shell, word, " ");
	free(temp);
	if (sep == '|' && (!word || !(*word)))
	{
		free(word);
		word = malloc(sizeof(char) * 2);
		if (!word)
			return (NULL);
		word = "|";
	}
	return (word);
}


/*
put word until separator, and skip quotes
if sep is | and a word is missing, then replace word with | */
bool	put_words(t_minishell *shell, char *s, char sep, char **array)
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
		if (!s[i] || (sep == '|' && (s[i] == sep || !s[i + 1])) || (sep == ' '
				&& s[i] != sep && (s[i + 1] == sep || !s[i + 1])))
		{
			if (sep == ' ' || (sep == '|' && (!s[i + 1] || i == 0)))
				array[word] = ft_substr(shell, s, start, i - start + 1);
			else
				array[word] = ft_substr(shell, s, start, i - start);
			array[word] = replace_empty_word(shell, array[word], sep);
			word++;
			start = i + 1;
		}
		i++;
	}
	return (true);
}

char	**split_skip_quotes(t_minishell *shell, char *s, char sep)
{
	int		words;
	char	**array;
	int	i;

	words = count_words(shell->usr_input, sep);
	array = NULL;
	array = malloc((words + 1) * sizeof(char *));
	if (!array)
		error_free_all(shell, ERR_MALLOC);
	i = 0;
	while (i < words + 1)
	{
		array[i] = NULL;
		i++;
	}
	if (!put_words(shell, s, sep, array))
		error_free_all(shell, ERR_MALLOC);
	return (array);
}
/* int	main(void)
{
	char **array = ft_split("echo \"ls -l | grep m\" | grep i", '|');

	printf("amount of words: %d\n", words);
	int i = 0;
	while (array[i])
	{
		printf("word at index %d: %s\n", i, array[i]);
		i++;
	}
} */