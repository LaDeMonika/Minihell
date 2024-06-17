#include "../../inc/minishell.h"

int	count_literal_chars(char *str)
{
    char metaquote;
	int	i;
	int	len;

    metaquote = '\0';
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

void    copy_char_and_increment_new_str(char *new_str, char c, int* j)
{
    new_str[*j] = c;
    (*j)++;
}

char    *write_literal_chars(char *str, char *new_str)
{
    char metaquote;
    int i;
    int j;

    metaquote = '\0';
    i = 0;
    j = 0;
    while (str && str[i])
    {
        if (str[i] == '"' || str[i] == '\'')
        {
            if (!(metaquote))
                metaquote = str[i];
            else if (metaquote == str[i])
                metaquote = '\0';
            else if (str[i] != metaquote)
                copy_char_and_increment_new_str(new_str, str[i], &j);
        }
        else
            copy_char_and_increment_new_str(new_str, str[i], &j);
        i++;
    }
    return (new_str);
}

/*first determine new string length and then create new string without outer quotes*/
char *remove_metaquotes(t_minishell *shell, char *str)
{
    int new_len;
    char    *new_str;

    new_len = count_literal_chars(str);
    new_str = try_malloc(shell, sizeof(char) * (new_len + 1));
    new_str[new_len] = '\0';
    new_str = write_literal_chars(str, new_str);
    free_and_reset_ptr((void **)&str);
    return (new_str);
}

char    **fill_array_with_null(char **array, int size)
{
    int i;

    i = 0;
    while (i < size)
    {
        array[i] = NULL;
        i++;
    }
    return (array);
}
