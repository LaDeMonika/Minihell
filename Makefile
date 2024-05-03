NAME = minishell

CFLAGS = -Wall -Werror -Wextra -g
INCLDS = inc/minishell.h
LIBFTDIR = ./libft
LIBFTNAME = $(LIBFTDIR)/libft.a
LIBS =  -L$(LIBFTDIR) -lft
EXT_LIBS = -lreadline

SRCS = main/minishell.c main/init_shell.c main/redirections.c main/signals.c main/prompt.c main/command.c main/pipes.c main/heredoc.c

BUILTIN = builtins/builtin.c builtins/ft_echo.c

UTILS = utils/utils_00.c utils/get_next_line_utils.c utils/get_next_line.c

ERR = err/error_msg.c

SRC = $(SRCS) $(ERR) $(BUILTIN) $(UTILS)

OBJDIR = obj/
OBJS = $(addprefix $(OBJDIR), $(SRC:.c=.o))

all: makelibft $(NAME)

$(OBJDIR)%.o: src/%.c $(INCLDS)
	mkdir -p $(dir $@)
	@$(CC) -c $(CFLAGS) $(DEPFLAGS) $< -o $@

$(NAME): $(LIBFTNAME) $(OBJS)
	cc $(CFLAGS) $(OBJS) $(LIBS) $(EXT_LIBS)  -o $(NAME)

makelibft:
	cd $(LIBFTDIR) && make

clean:
	rm -f $(OBJS)
	cd $(LIBFTDIR) && make clean

fclean: clean
	rm -f $(NAME)
	rm -rf $(OBJDIR)
	cd $(LIBFTDIR) && make fclean

re: fclean all

.PHONY: all re clean fclean makelibft