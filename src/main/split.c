#include "../../inc/minishell.h"
#include <stdbool.h>

bool	is_space(char c)
{
	if ((c >= '\t' && c <= '\r') || c == ' ')
		return (true);
	return (false);
}

/*
if sep is pipe start word count at one and at one for every pipe
if sep is space,
	then start word count at zero and only add for space before other character or null-terminator*/
static int	count_words(char *s, char sep)
{
	int	i;
	int	words;

	i = 0;
	words = 0;
	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
			i = skip_between_metaquotes(s, i, s[i]);
		if (NEW_WORD_ON_PIPE || NEW_WORD_ON_SPACE || NEW_WORD_ON_COLON)
			words++;
		i++;
	}
	return (words);
}
/*
if sep i ' ' then increase len by one so that it also puts the current char*/
static char	*parse_word(t_minishell *shell, char *word, char sep)
{
	char	*temp;

	temp = word;

	word = ft_strtrim(shell, word, " \f\n\r\t\v");

	free_and_reset_ptr((void **)&temp);
	if (sep == '|' && (!word || !(*word)))
	{

		free_and_reset_ptr((void **)&word);

		word = try_malloc(shell, sizeof(char) * 2);
		word[0] = '|';
		word[1] = '\0';
	}
	return (word);
}

/*
put word until separator, and skip quotes
if sep is | and a word is missing, then replace word with | */
void	put_words(t_minishell *shell, char *s, char sep, char **array)
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
			i = skip_between_metaquotes(s, i, s[i]);
		if (!s[i] || NEW_WORD_ON_PIPE || NEW_WORD_ON_SPACE || NEW_WORD_ON_COLON)
		{
			if (sep == ' ' || (sep == '|' && (!s[i + 1] || i == 0)))
				array[word] = ft_substr(shell, s, start, i - start + 1);
			else
				array[word] = ft_substr(shell, s, start, i - start);
			array[word] = parse_word(shell, array[word], sep);
			word++;
			start = i + 1;
		}
		i++;
	}
}

char	**split_while_skipping_quotes(t_minishell *shell, char *s, char sep)
{
	int		words;
	char	**array;
	int		i;

	words = count_words(s, sep);
	array = NULL;
	if (sep == '|')
		array = shell->input_array;
	else if (sep == ' ')
		array = shell->command_array;
	else if (sep == ':')
		array = shell->path_array;

	array = try_malloc(shell, (words + 1) * sizeof(char *));
	i = 0;
	while (i < words + 1)
	{
		array[i] = NULL;
		i++;
	}

	put_words(shell, s, sep, array);
	/* printf("sep: %c amount of words: %d\n", sep, words);
	int j = 0;
	while (array[j])
	{
		printf("word at index %d: %s\n", j, array[j]);
		j++;
	} */
	return (array);
}
/* int	main(void)
{
	char **array = ft_split("echo \"ls -l | grep m\" | grep i", '|');

	printf("sep: %c amount of words: %d\n", sep, words);
	int j = 0;
	while (array[j])
	{
		printf("word at index %d: %s\n", j, array[j]);
		j++;
	}
} */