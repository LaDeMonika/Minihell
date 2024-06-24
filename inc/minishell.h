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
#include <sys/stat.h>

/**************************DEFINES***************************/
#define	ON_PIPE (sep == '|' && (s[i] == sep || !s[i + 1]))
#define	ON_SPACE (sep == ' ' && !is_space(s[i]) && (is_space(s[i + 1]) || !s[i + 1]))
#define	ON_COLON (sep == ':' && (s[i] == sep || !s[i + 1]))
#define	ON_FORCEWRITE (sep == '|' && s[i] == '>' && s[i + 1] == '|')
/* REPLACE WITH */
/*
static inline int ON_PIPE(char sep, char *s, int i) {
    return (sep == '|' && (s[i] == sep || !s[i + 1]));
}

static inline int FORCEWRITE(char sep, char *s, int i) {
    return (sep == '|' && s[i] == '>' && s[i + 1] == '|');
}

static inline int ON_SPACE(char sep, char *s, int i) {
    return (sep == ' ' && !is_space(s[i]) && (is_space(s[i + 1]) || !s[i + 1]));
}

static inline int ON_COLON(char sep, char *s, int i) {
    return (sep == ':' && (s[i] == sep || !s[i + 1]));
}
*/

/**************************STRUCT****************************/
typedef struct s_token_list
{
	char					*token;
	int						sep;
	struct s_token_list		*next;
}							t_token_list;

typedef struct s_minishell
{
	char					*usr_input;
	char					**input_array;
	int						pipes_total;
	int						*pid;
	t_token_list			**list;
	bool					stay_in_parent;
	int						parsing_exit_status;
	int						last_exit_status;
	char					*prompt;
	char					**envp;
	int						line_count;
	char					*input_file;
	char					**command_array;
	char					**path_array;
	char					*unexpected_token;
	char	**env_subarray;
	struct sigaction		sa_sigint;
	struct sigaction		sa_sigquit;
	int						pipe_fd[2];
	int						status;
	int						pre_sep;
	int						post_sep;
	int						builtin;
	char	*new_key;
	char						*new_value;
	char	**new_envp;
	char	*env_key;
	char	*env_value;
	char	*my_pid;
	char	**split_array;
	char	*base;
	char	*suffix;
	char	*path;
	char	*command_path;
	char	*heredoc_input;
	char *new_pwd;
	int	my_exit_status;
	int	custom_errno;
	char	*temp_str;
	char		cwd[PATH_MAX];
	char	metaquote;
	char	*token;
	char	*last_arg;
}							t_minishell;


/****************************ENUM****************************/
enum						e_error
{
	EXIT,
	ERR_MALLOC,
	ERR_TOO_MANY_ARGS,
	ERR_TOO_FEW_ARGS,
	ERR_INvalid_key,
	ERR_PATH_NOT_FOUND,
	ERR_SIGEMPTYSET,
	ERR_SIGACTION,
	ERR_OPEN,
	ERR_DUP2,
	ERR_READ,
	ERR_WRITE,
	ERR_CLOSE,
	ERR_UNLINK,
	ERR_PIPE,
	ERR_FORK,
	ERR_WAITPID,
	ERR_GETPID,
	ERR_GETCWD,
	ERR_STAT
};

enum e_custom_errno
{
	U_TOO_MANY_ARGUMENTS,
	U_INVALID_IDENTIFIER,
	U_INVALID_OPTION,
	U_NO_FILE,
	U_NO_PERMISSION,
	U_NO_FILENAME_ARGUMENT,
	U_IS_DIR,
	U_IS_DIRECTORY

};

enum						e_token_sep
{
	INPUT,
	OUTPUT,
	HEREDOC,
	APPEND,
	COMMAND,
	INVALID_PIPE,
	FORCEWRITE
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

// signal handling modes
enum						e_open_mode
{
	WRITE_TRUNCATE,
	WRITE_APPEND,
	READ
};

// builtins
enum						e_builtin
{
	B_ECHO,
	B_CD,
	B_PWD,
	B_EXPORT,
	B_UNSET,
	B_ENV,
	B_EXIT,
	NOT_BUILTIN
};

//freeing in append function
enum	e_free_afterwards
{
	BASE,
	SUFFIX,
	BOTH
};
/*************************PROTOTYPES*************************/
//********************src/main
char	*tester_readline(t_minishell *shell, char *prompt);

// command
char	*find_command_in_path_env(t_minishell *shell);
void						execute_command(t_minishell *shell, char *command);
int	execute_command_array(t_minishell *shell, bool print_message);

// main
void						handle_input(t_minishell *shell);
//exit status
void	set_exit_status_before_termination(t_minishell *shell, int *exit_status, int custom_errno);
void						set_exit_status_after_termination(
								t_minishell *shell, int *child_status,
								int remaining_children);
void	set_custom_errno(t_minishell *shell, int custom_errno, int exit_status);

// init_shell_struct
void						init_shell_struct(t_minishell *shell,
								int argc, char **envp);
void						init_input_iteration(t_minishell *shell);

//environment
char	*get_env_value(t_minishell *shell, char *base, int *start, int *i);

// err
void						error_free_all(t_minishell *shell, int err,
								char *prefix, char *custom_message);
void						free_and_reset_ptr(void **ptr);
void	free_and_reset_array(void ***array, bool is_list);
int	free_all(t_minishell *shell);
void						print_error(char *prefix, char *custom_message);
void	free_iteration(t_minishell *shell);
void	error_free_all_exit(t_minishell *shell, int exit_status);


// prompt

void						append_path(t_minishell *shell);
void						append_hostname(t_minishell *shell);
void						build_prompt(t_minishell *shell);

//tokenize
void						extract_and_add_tokens(t_minishell *shell,
								int index, int start, int len);
void						list_add(t_minishell *shell, t_token_list **head,
								char *token);
void	append_to_command(t_minishell *shell, t_token_list **head,
		char *command_arg, char **token);
void						tokenize(t_minishell *shell, char *command,
								int index);
								char	*prepare_last_arg(t_minishell *shell, char *str);

// pipes
void						handle_pipes(t_minishell *shell,
								char **input_array, int pipes_left,
								int read_fd);
void						parent(t_minishell *shell, char **input_array,
								int pipes_left, int read_fd);
void						child(t_minishell *shell, int pipes_left,
								int read_fd);
// parse + heredoc
void						parse_input(t_minishell *shell);
void						heredoc(t_minishell *shell, char **eof,
								char *input_buffer);
char	*extract_line(t_minishell *shell, char *input, char **heredoc_input);
int	parse_token(t_minishell *shell, t_token_list *list);

// redirections
int							find_redirect(char *command, int i);
void						redirect_stream(t_minishell *shell, char *file,
								int mode, int fd2);
void	handle_redirections(t_minishell *shell, t_token_list *list, int index);
char						*remove_metaquotes(t_minishell *shell,
								char *command);
// signals
void						child_sigint_handler(int sig);
void						child_sigquit_handler(int sig);
void						parent_sigint_handler(int sig);

void						set_signals(t_minishell *shell, int mode);

// preprocess/expand
char	*extract_and_append(t_minishell *shell, char *base, int len);
char						*append_heredoc_on_missing_quote(t_minishell *shell,
								char *old_s);
char						*expand_env_variables(t_minishell *shell, char *s);
int							skip_between_metaquotes(char *str, int i,
								char metaquote);
char						*check_env_variables(t_minishell *shell, char *s);
char	**split_while_skipping_quotes(t_minishell *shell,
								char *s, char sep);
char	*ft_getenv(t_minishell *shell, char *key);
void	handle_heredoc(t_minishell *shell, t_token_list *list, int error_at_index);

//try
int							try_read(t_minishell *shell, int fd, char **buffer,
								char *file);
int							try_write(t_minishell *shell, int fd, char *buffer,
								int bytes);
void						try_close(t_minishell *shell, int fd);
int	try_open(t_minishell *shell, char *file, int mode);
void						*try_malloc(t_minishell *shell, int size);
void						try_dup2(t_minishell *shell, int fd, int fd2);
void						try_pipe(t_minishell *shell, int fd[2]);
int							try_fork(t_minishell *shell);
void	try_getcwd(t_minishell *shell);

//********************src/builtins
bool	is_builtin(t_minishell *shell, char *token);
int							ft_strcmp(char *s1, char *s2);
int handle_builtin(t_minishell *shell, int *custom_errno);
char						ft_strcmp_btin(char *s1, char *s2);
int							ft_echo(t_minishell *shell);
int ft_cd(t_minishell *shell, int *custom_errno);
int ft_env(t_minishell *shell, int *custom_errno);
int ft_unset(t_minishell *shell, char *key, int *custom_errno);
int							ft_exit(t_minishell *shell, int *custom_errno);
int ft_pwd(t_minishell *shell);
int ft_export(t_minishell *shell, char *arg, int *custom_errno);
int export_no_args(t_minishell  *shell);
int index_of_char(char *str, char c);
char *update_value(t_minishell *shell, char *key, char *value, bool append);

//********************src/utils
// strings_1
char						*ft_strtrim(t_minishell *shell, char const *s1,
								char const *set);
char						*ft_strjoin(t_minishell *shell, char const *s1,
								char const *s2);
char	*ft_strchr(const char *s, char c);
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
char	*ft_strdup(t_minishell *shell, const char	*s);

// strings_3
char						*ft_itoa(t_minishell *shell, int n);
int							skip_first_metaquote_pair(char *str);
bool						has_even_metaquotes(char *s);
char						*append(t_minishell *shell, char *base,
								char *suffix, int free_afterwards);

// strings_4

bool						is_space(char c);
int							ft_atoi(const char	*nptr, bool *is_valid_number);


//strings_5
bool    					ft_isalpha(char c);
int 						sizeof_array(void **array);

// strings_6
char 						*remove_metaquotes(t_minishell *shell, char *str);
int	count_literal_chars(char *str);
char    **fill_array_with_null(char **array, int size);
int count_occurences_of_char(char *str, char c);
char	*last_word(char *str);
int	calculate_len(int n);
void	write_number(int n, char *str, int len);

void	copy_char_and_increment_new_str(char *new_str, char c, int *j);

#endif