#ifndef MINISHELL_H
# define MINISHELL_H

/**************************INCLUDES**************************/
# include "../libft/libft.h"
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <limits.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <fcntl.h>
# include <linux/limits.h>
# include <stdbool.h>
# include <errno.h>

/**************************DEFINES***************************/
# define INPUT 0
# define OUTPUT 1
# define HEREDOC 2
# define APPEND 3
# define COMMAND 4

/**************************STRUCT****************************/
typedef struct s_command_list
{
    char *command_part;
    int delimiter;
    bool    primary_input;
    struct s_command_list  *next;
}   t_command_list;

typedef struct s_minishell
{
    char    *user;
    char    *usr_input;
    char    *prompt;
    int     len_prompt;
    int     *pid;
    int     exit_status;
    char    **path_array; // Array of paths
    char    *home_dir;
    char    *current_dir; // Current working directory
    char    *command_path; // Path to the command
    char    **envp; // Environment variables
    char    **input_array; // Array of input arguments
    char    **command_history; // Array of command history
    int     history_index; // Index of the command history
    int     last_exit_status; // Exit status of the last command
    int     fd_hostname; // File descriptor for hostname
    int     fd; // File descriptor
    int status;
    struct sigaction    sa_sigint;
    struct sigaction    sa_sigquit;
    int pipes_total;
    int pipe_fd[2];
    char *line_count;
}               t_minishell;

/****************************ENUM****************************/
enum e_error
{
    ERR_MALLOC,
    ERR_TOO_MANY_ARGS,
    ERR_TOO_FEW_ARGS,
    ERR_INVALID_ARG,
    ERR_PATH_NOT_FOUND,
    NOT_BUILTIN,
    ERR_SIGEMPTYSET,
    ERR_SIGACTION,
    ERR_OPEN,
    ERR_READ,
    ERR_CLOSE
};

/*************************PROTOTYPES*************************/
//********************src/main
//command
void	execute_command(t_minishell *shell, char *command, char **envp);
void	extract_command_part(char *command, int start, int len,
		int preceding_delimiter, t_command_list **list);
//main
char	*find_command(char **input_array);
void	custom_perror(char *prefix, char *custom_message);
char	*set_exit_status(int *exit_status);
void	list_add(t_command_list **head, char *command_part, int type);
void	append_to_command(t_command_list **head, char *command_part);
void	handle_delimiters(t_minishell *shell, char *command, char **envp);
void	parent(t_minishell *shell, char **input_array, int pipes_left,
		int read_fd);
void	child(t_minishell *shell, char **input_array, int pipes_left,
		int read_fd);
void	handle_pipes_recursive(t_minishell *shell, char **input_array,
		int pipes_left, int read_fd);
void	handle_pipes(t_minishell *shell, int read_fd);
void	handle_input(t_minishell *shell);
char	**ft_split_ignore_quotes(t_minishell *shell, char *s, char c);
void    add_to_line_count(char *old_line_count, int lines);
void	read_line_count(t_minishell *shell);

//init_shell_struct
void	init_shell_struct(t_minishell *shell, char **envp);

//err
void    ft_error_msg(char err);
void	error_free_exit(t_minishell *shell, char err);

//prompt
char	*append_to_prompt(t_minishell *shell, char *s);
void    append_path(t_minishell *shell);
void    append_hostname(t_minishell *shell);
void	build_prompt(t_minishell *shell);

//redirections
void	append_output(char *output_file);
void	heredoc_input(t_minishell *shell, char *eof);
int	    find_delimiter(char c1, char c2);
void	redirect_input(char *input_file);
void	redirect_output(char *output_file);
void	heredoc_execute(t_minishell *shell, char *eof);
void	handle_redirections(t_minishell *shell, t_command_list *list, char **envp);

//signals
void	child_sigint_handler(int sig);
void	child_sigquit_handler(int sig);
void	sigint_handler(int sig);
void	set_child_signals(t_minishell *shell);
void	set_last_exit_status(t_minishell *shell);
void	set_signals_parent(t_minishell *shell);

//********************src/builtins
int ft_is_builtin(t_minishell *shell, char **command_array);
int ft_echo(char **command_array);

//********************src/utils
//utils_00
char	*append_affix(char *old_s, int start, int len, char *new_s);
char    *check_env_variables(t_minishell *shell, char *s);
char	**ft_split_ignore_quotes(t_minishell *shell, char *s, char c);
//utils_01

#endif