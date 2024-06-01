#include "../../inc/minishell.h"

int	count_literal_chars(char *str, char metaquote)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (str && str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			if (!(metaquote))
				metaquote = str[i];
			else if (metaquote == str[i])
				metaquote = '\0';
			else if (str[i] != metaquote)
				len++;
		}
		else
			len++;
		i++;
	}
	return (len);
}

char *initialize(t_minishell *shell, char *str, char *metaquote, int *new_len, int *i, int *j)
{
    *metaquote = '\0';
    *new_len = count_literal_chars(str, *metaquote);
    char *new_str = try_malloc(shell, sizeof(char) * (*new_len + 1));
    *i = 0;
    *j = 0;
    return new_str;
}

void process_string(char *str, char *new_str, char *metaquote, int *i, int *j)
{
    while (str && str[*i])
    {
        if (str[*i] == '"' || str[*i] == '\'')
        {
            if (!(*metaquote))
                *metaquote = str[*i];
            else if (*metaquote == str[*i])
                *metaquote = '\0';
            else if (str[*i] != *metaquote)
            {
                new_str[*j] = str[*i];
                (*j)++;
            }
        }
        else
        {
            new_str[*j] = str[*i];
            (*j)++;
        }
        (*i)++;
    }
}

char *finalize(char *new_str, char *str, int j)
{
    new_str[j] = '\0';
    free_and_reset_ptr((void **)&str);
    return new_str;
}

char *remove_metaquotes(t_minishell *shell, char *str)
{
    char metaquote;
    int new_len, i, j;
    char *new_str = initialize(shell, str, &metaquote, &new_len, &i, &j);
    process_string(str, new_str, &metaquote, &i, &j);
    return finalize(new_str, str, j);
}
