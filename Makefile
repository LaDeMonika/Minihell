NAME = minishell

CFLAGS = -Wall -Werror -Wextra -g
INCLDS = inc/minishell.h
EXT_LIBS = -lreadline

SRCS = main/minishell.c main/init_shell.c \
		main/redirections.c main/signals.c \
		main/prompt.c main/execute.c main/pipes.c \
		main/parse.c main/tokenize.c main/preprocess.c \
		main/split.c main/try.c main/exit_status.c\
		main/heredoc.c main/environment.c \

BUILTIN = builtins/builtin.c builtins/ft_echo.c \
			builtins/ft_cd.c builtins/ft_pwd.c \
			builtins/ft_env.c builtins/ft_unset.c \
			builtins/ft_exit.c builtins/ft_export.c

UTILS = utils/strings_1.c utils/strings_2.c \
			utils/strings_3.c utils/strings_4.c \
			utils/strings_5.c utils/strings_6.c

ERR = err/error_msg_00.c err/error_msg_01.c

TEST = get_next_line/get_next_line.c

SRC = $(SRCS) $(ERR) $(BUILTIN) $(UTILS) $(TEST)

OBJDIR = obj/
OBJS = $(addprefix $(OBJDIR), $(SRC:.c=.o))

all: $(NAME)

$(OBJDIR)%.o: src/%.c $(INCLDS)
	mkdir -p $(dir $@)
	@$(CC) -c $(CFLAGS) $< -o $@

$(NAME): $(OBJS)
	cc $(CFLAGS) $(OBJS) $(EXT_LIBS)  -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)
	rm -rf $(OBJDIR)

re: fclean all

.PHONY: all re clean fclean