#ifndef MINISHELL_H
# define MINISHELL_H

//***************************INCLUDES
# include "../libft/libft.h"
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <limits.h>
# include <sys/wait.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <fcntl.h>
# include <linux/limits.h>
# include <stdbool.h>
# include <errno.h>

# define INPUT 0
# define OUTPUT 1
# define HEREDOC 2
# define APPEND 3
# define COMMAND 4

typedef struct s_command_list
{
    char *command_part;
    int delimiter;
    bool    primary_input;
    struct s_command_list  *next;
}   t_command_list;

//***************************STRUCT
typedef struct s_minishell
{
    char    *user;
    char    *usr_input;
    char    *prompt;
    int     len_prompt;
    int   *pid;
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
}               t_minishell;

//***************************ENUM
/* enum e_return
{
    RET_SUCCESS,
    RET_FAILURE
}; */

enum e_error
{
    ERR_MALLOC,
    ERR_TOO_MANY_ARGS,
    ERR_TOO_FEW_ARGS,
    ERR_INVALID_ARG,
    ERR_PATH_NOT_FOUND,
    NOT_BUILTIN
};

//***************************PROTOTYPES
// main
void	execute_command(t_minishell *shell, char *command, char **envp);
void	init_shell_struct(t_minishell *shell, char **envp);
char	**ft_split_ignore_quotes(t_minishell *shell, char *s, char c);
// builtins
int     ft_echo(t_minishell *shell);
int     ft_is_builtin(t_minishell *shell);
// err
void    ft_error_msg(char err);

//pipes
void	handle_pipes_recursive(t_minishell *shell, char **input_array, int pipes_left, int read_fd);

//redirections
void	append_output(char *output_file);
void	heredoc_input(char *input_file);
int	    find_delimiter(char c1, char c2);
void	redirect_input(char *input_file);
void	redirect_output(char *output_file);
void	handle_redirections(t_minishell *shell, t_command_list *list, char **envp);

//signals
void	handle_signals(t_minishell *shell);
void	sigint_handler(int sig);
void	child_sigint_handler(int sig);
void	child_sigquit_handler(int sig);
void	set_child_signals(t_minishell *shell);
void	set_last_exit_status(t_minishell *shell);

//utils


#endif