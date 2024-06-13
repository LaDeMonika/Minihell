#include "../../inc/minishell.h"

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
		if (SKIP_FORCE_WRITE)
			i++;
		else if (!s[i] || NEW_WORD_ON_PIPE || NEW_WORD_ON_SPACE || NEW_WORD_ON_COLON)
			words++;
		if (s[i])
			i++;
	}
	return (words);
}

static char	*parse_word(t_minishell *shell, char *word, char sep)
{
	word = ft_strtrim(shell, word, " \f\n\r\t\v");
	if (sep == '|' && (!word || !(*word)))
	{

		free_and_reset_ptr((void **)&word);
		word = try_malloc(shell, sizeof(char) * 2);
		word[0] = '|';
		word[1] = '\0';
	}
	return (word);
}

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
		if (SKIP_FORCE_WRITE)
			i++;
		else if (!s[i] || NEW_WORD_ON_PIPE || NEW_WORD_ON_SPACE || NEW_WORD_ON_COLON)
		{
			if (s[i] && (sep == ' ' || (sep == '|' && (!s[i + 1] || i == 0))))
				array[word] = ft_substr(shell, s, start, i - start + 1);
			else
				array[word] = ft_substr(shell, s, start, i - start);
			array[word] = parse_word(shell, array[word], sep);
			word++;
			start = i + 1;
		}
		if (s[i])
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
	array = try_malloc(shell, (words + 1) * sizeof(char *));
	i = 0;
	while (i < words + 1)
	{
		array[i] = NULL;
		i++;
	}
	put_words(shell, s, sep, array);
	return (array);
}
