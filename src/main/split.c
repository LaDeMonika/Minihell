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

void	put_words(t_minishell *shell, char *s, char sep)
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
				shell->split_array[word] = ft_substr(shell, s, start, i - start + 1);
			else
				shell->split_array[word] = ft_substr(shell, s, start, i - start);
			shell->split_array[word] = parse_word(shell, shell->split_array[word], sep);
			word++;
			start = i + 1;
		}
		if (s[i])
			i++;
	}
}

void split_while_skipping_quotes(t_minishell *shell, char *s, char sep)
{
	int		words;

	words = count_words(s, sep);
	shell->split_array = NULL;
	shell->split_array = try_malloc(shell, (words + 1) * sizeof(char *));
	shell->split_array = fill_array_with_null(shell->split_array, words + 1);
	put_words(shell, s, sep);
	if (sep == '|')
		shell->input_array = shell->split_array;
	else if (sep == ' ')
		shell->command_array = shell->split_array;
	else if (sep == ':')
		shell->path_array = shell->split_array;
	shell->split_array = NULL;
}
