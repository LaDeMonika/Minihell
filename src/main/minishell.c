#include "../../inc/minishell.h"
#include <unistd.h>

void	handle_input(t_minishell *shell)
{
	int	i;



	shell->usr_input = append_heredoc_on_missing_quote(shell, shell->usr_input);
	//shell->usr_input = expand_env_variables(shell, shell->usr_input);
	shell->input_array = split_while_skipping_quotes(shell, shell->usr_input,
			'|');
	while (shell->input_array[shell->pipes_total + 1])
		shell->pipes_total++;
	shell->pid = try_malloc(shell, sizeof(int) * (shell->pipes_total + 2));
	shell->list = try_malloc(shell, sizeof(t_token_list *) * (shell->pipes_total
				+ 2));
	shell->list[shell->pipes_total + 1] = NULL;
	i = 0;
	while (shell->input_array[i])
	{
		shell->list[i] = NULL;
		tokenize(shell, shell->input_array[i], i);
		i++;
	}
	parse_input(shell);
	if (shell->pipes_total == 0
	&& (ft_strncmp(shell->list[0]->token, "cd",	3) == 0
	|| ft_strncmp(shell->list[0]->token, "cd ", 3) == 0
	|| ft_strncmp(shell->usr_input, "exit", 5) == 0
	|| ft_strncmp(shell->usr_input, "exit ", 5) == 0))
	{
		/* printf("command token is: %s\n", shell->list[0]->token);
		printf("cd without argument? %d\n", ft_strncmp(shell->list[0]->token, "cd",
				3) == 0);
		printf("cd with argument? %d\n", ft_strncmp(shell->list[0]->token, "cd ",
				3) == 0);
		printf("doing command in parent\n"); */
		shell->stay_in_parent = true;
		handle_redirections(shell, shell->list[0], STDIN_FILENO);
	}
	else if (shell->parsing_exit_status == 0)
	{
		/* printf("making children\n"); */
		handle_pipes_recursive(shell, shell->input_array, shell->pipes_total,
			STDIN_FILENO);
	}

	else
		shell->last_exit_status = shell->parsing_exit_status;
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	*shell;

	(void)argv;
	shell = NULL;
	shell = malloc(sizeof(t_minishell));
	if (!shell)
		return (perror("malloc"), 1);
	init_shell_struct(shell, envp);
	if (argc > 1)
		return (error_free_all(shell, ERR_TOO_MANY_ARGS, NULL, NULL), 1);
	while (1)
	{
		init_input_iteration(shell);
		set_signals(shell, PARENT_WITHOUT_CHILD);
		build_prompt(shell);
		shell->usr_input = readline(shell->prompt);
		if (!shell->usr_input)
			return (free_all(shell), 0);
		if (ft_strncmp(ft_strtrim(shell, shell->usr_input, " "), "\0", 1) != 0)
		{
			add_history(shell->usr_input);
			handle_input(shell);
		}

	}
	free_all(shell);
}

//main for tester:
/* 
#include "../get_next_line/get_next_line.h"
int	main(int argc, char **argv, char **envp)
{
	t_minishell	*shell;

	(void)argv;
	shell = NULL;
	shell = malloc(sizeof(t_minishell));
	if (!shell)
		return (perror("malloc"), 1);
	init_shell_struct(shell, envp);
	if (argc > 1)
		return (error_free_all(shell, ERR_TOO_MANY_ARGS, NULL, NULL), 1);
	while (1)
	{
		init_input_iteration(shell);
		set_signals(shell, PARENT_WITHOUT_CHILD);
		build_prompt(shell);
		if (isatty(fileno(stdin)))
			shell->usr_input = readline(shell->prompt);
		else
		{
			char *line;
			line = get_next_line(fileno(stdin));
			shell->usr_input = ft_strtrim(shell, line, "\n");
			free(line);
		}
		if (!shell->usr_input)
			return (free_all(shell), shell->last_exit_status);
		if (ft_strncmp(ft_strtrim(shell, shell->usr_input, " \n\f\r\t\v"), "\0", 1) != 0)
		{
			add_history(shell->usr_input);
			handle_input(shell);
		}

	}
	free_all(shell);
} */