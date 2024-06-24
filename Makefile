NAME = minishell

CFLAGS = -Wall -Werror -Wextra -g
INCLDS = inc/minishell.h
EXT_LIBS = -lreadline

SRCS = main/minishell.c main/init.c \
		main/redirections.c main/signals.c \
		main/prompt.c main/execute.c main/pipes.c \
		main/parse.c main/tokenize.c main/preprocess.c \
		main/split.c main/exit_status.c\
		main/heredoc.c main/environment.c \
		main/free.c

BUILTIN = builtins/builtin.c builtins/ft_echo.c \
			builtins/ft_cd.c builtins/ft_pwd.c \
			builtins/ft_env.c builtins/ft_unset.c \
			builtins/ft_exit.c builtins/ft_export.c

UTILS = utils/array.c \
		utils/check_string.c \
		utils/check_string_2.c \
		utils/check_string_3.c \
		utils/convert_string.c \
		utils/create_string.c \
		utils/fill_string.c \
		utils/quotes_read.c \
		utils/quotes_write.c \
		utils/try_io.c utils/try_utils.c \

ERR = err/error.c

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