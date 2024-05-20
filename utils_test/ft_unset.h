#ifndef FT_UNSET_H
# define FT_UNSET_H

/**************************INCLUDES**************************/
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

/**************************STRUCT****************************/
typedef struct s_token_list
{
	char					*token;
	int						delimiter;
	struct s_token_list	*next;
}							t_token_list;

typedef struct s_minishell
{
	char					*user;
	char					*usr_input;
	char					*prompt;
	int						len_prompt;
	int						*pid;
	int						exit_status;
	char **path_array;
	char					*home_dir;
	char 					*current_dir;
	char 					*command_path;
	char 					**envp;
	char 					**input_array;
	char 					**command_history;
	int history_index;
	int last_exit_status;
	int fd_hostname;
	int fd;
	int						status;
	struct sigaction		sa_sigint;
	struct sigaction		sa_sigquit;
	int						pipes_total;
	int						pipe_fd[2];
	int						line_count;
	char					*input_file;
	int						parsing_exit_status;
	t_token_list			**list;
	int						pre_delimiter;
	int						post_delimiter;
	char					*unexpected_token;
	char **command_array;
}							t_minishell;

/*************************PROTOTYPES*************************/

#endif