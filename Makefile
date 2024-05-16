NAME = minishell

CFLAGS = -Wall -Werror -Wextra -g
INCLDS = inc/minishell.h
EXT_LIBS = -lreadline

SRCS = main/minishell.c main/init_shell.c main/redirections.c main/signals.c main/prompt.c main/command.c main/pipes.c main/parse.c main/tokenize.c main/preprocess.c main/split.c

BUILTIN = builtins/builtin.c builtins/ft_echo.c

UTILS = utils/strings_1.c utils/strings_2.c utils/strings_3.c utils/strings_4.c

ERR = err/error_msg.c

SRC = $(SRCS) $(ERR) $(BUILTIN) $(UTILS)

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