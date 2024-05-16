#ifndef MINISHELL_H
# define MINISHELL_H

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

/**************************DEFINES***************************/
# define NEW_WORD_ON_PIPE (sep == '|' && (s[i] == sep || !s[i + 1]))
# define NEW_WORD_ON_SPACE (sep == ' ' && s[i] != sep && (s[i + 1] == sep || !s[i + 1]))
# define NEW_WORD_ON_COLON (sep == ':' && (s[i] == sep || !s[i + 1]))

/**************************STRUCT****************************/
typedef struct s_command_list
{
	char					*token;
	int						delimiter;
	struct s_command_list	*next;
}							t_token_list;

typedef struct s_minishell
{
	char					*user;
	char					*usr_input;
	char					*prompt;
	int						len_prompt;
	int						*pid;
	int						exit_status;
	char **path_array; // Array of paths
	char					*home_dir;
	char *current_dir;      // Current working directory
	char *command_path;     // Path to the command
	char **envp;            // Environment variables
	char **input_array;     // Array of input arguments
	char **command_history; // Array of command history
	int history_index;      // Index of the command history
	int last_exit_status;   // Exit status of the last command
	int fd_hostname;        // File descriptor for hostname
	int fd;                 // File descriptor
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

/****************************ENUM****************************/
enum						e_error
{
	NO_ERROR,
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
	ERR_CLOSE,
	ERR_GETPID
};

// token delimiters
enum						e_token_delimiter
{
	INPUT,
	OUTPUT,
	HEREDOC,
	APPEND,
	COMMAND,
	INVALID_PIPE
};

// signal handling modes
enum						e_signal_handling_mode
{
	PARENT_WITHOUT_CHILD,
	PARENT_WITH_CHILD,
	CHILD,
	HEREDOC_CHILD,
	PARENT_WITH_HEREDOC
};

/*************************PROTOTYPES*************************/
//********************src/main
// command
char						*find_command(t_minishell *shell,
								char **input_array);
void						execute_command(t_minishell *shell, char *command);
void						extract_and_add_tokens(t_minishell *shell,
								int index, int start, int len);
// main
void						custom_perror(char *prefix, char *custom_message);
char						*set_exit_status(t_minishell *shell,
								int *exit_status);
void						list_add(t_minishell *shell, t_token_list **head,
								char *token);
void						append_to_command(t_minishell *shell,
								t_token_list **head, char *command_arg,
								char *token);
void						tokenize(t_minishell *shell, char *command,
								int index);
void						parent(t_minishell *shell, char **input_array,
								int pipes_left, int read_fd);
void						child(t_minishell *shell, int pipes_left,
								int read_fd);
void						handle_pipes_recursive(t_minishell *shell,
								char **input_array, int pipes_left,
								int read_fd);
void						handle_input(t_minishell *shell);

// init_shell_struct
void						init_shell_struct(t_minishell *shell, char **envp);
void						init_input_iteration(t_minishell *shell);

// err
void						error_free_all(t_minishell *shell, int err);
void						free_and_reset_ptr(void **ptr);
void						free_and_reset_array(void ***array);
void						free_all(t_minishell *shell);
void						print_error(char *prefix, char *custom_error);

// prompt
char						*append_suffix(t_minishell *shell, char *base,
								char *suffix);
void						append_path(t_minishell *shell);
void						append_hostname(t_minishell *shell);
void						build_prompt(t_minishell *shell);

// pipes
void						handle_pipes(t_minishell *shell, int read_fd);

// hereoc
void						parse_input(t_minishell *shell);
void						heredoc(t_minishell *shell, char *eof,
								char *input_buffer);

// redirections

int							find_redirect(char *command, int i);
void						redirect_input(char *input_file, int read_fd);
void						redirect_output(char *output_file, int delimiter);
void						handle_redirections(t_minishell *shell,
								t_token_list *list, int read_fd);
char						*remove_metaquotes(t_minishell *shell,
								char *command);
// signals
void						child_sigint_handler(int sig);
void						child_sigquit_handler(int sig);
void						parent_sigint_handler(int sig);
void						set_child_exit_status(t_minishell *shell,
								int *child_status, int remaining_children);
void						set_signals(t_minishell *shell, int mode);

// preprocess
char						*extract_substr_and_append(t_minishell *shell,
								char *base, int len, char *new_str);
char						*append_heredoc_on_missing_quote(t_minishell *shell,
								char *old_s);
char						*expand_env_variables(t_minishell *shell, char *s);
int							skip_between_metaquotes(char *str, int i,
								char metaquote);
char						*check_env_variables(t_minishell *shell, char *s);
char						**split_skip_quotes(t_minishell *shell, char *s,
								char sep);

//********************src/builtins
int							ft_is_builtin(t_minishell *shell,
								char **command_array);
int							ft_echo(char **command_array);

//********************src/utils
// strings_1
char						*ft_strtrim(t_minishell *shell, char const *s1,
								char const *set);
char						*ft_strjoin(t_minishell *shell, char const *s1,
								char const *s2);
char						*ft_strchr(const char *s, int c);
char						*ft_strnstr(const char *big, const char *little,
								size_t len);
size_t						ft_strlcpy(char *dst, const char *src, size_t size);

// strings_2
char						*ft_substr(t_minishell *shell, char const *s,
								unsigned int start, size_t len);
size_t						ft_strlen(const char *s);
int							ft_strncmp(const char *s1, const char *s2,
								size_t n);
int							ft_isalnum(int c);
char						*ft_strdup(t_minishell *shell, const char *s);

// strings_3
char						*ft_itoa(t_minishell *shell, int n);
int							skip_first_metaquote_pair(char *str);
bool						has_even_metaquotes(char *s);

// strings_4
char						**ft_split(char const *s, char c);
void						ft_putstr_fd(char *s, int fd);

// utils_01

#endif