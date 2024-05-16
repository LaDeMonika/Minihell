<<<<<<<< HEAD:tmp/Makefile
CFLAGS = -Wall -Werror -Wextra -g
SRCS = src/main.c
INCLDS = inc/minishell.h
OBJS = $(SRCS:src/%.c=obj/%.o)
========
>>>>>>>> main:Makefile
NAME = minishell

CFLAGS = -Wall -Werror -Wextra -g
INCLDS = inc/minishell.h
LIBFTDIR = ./libft
LIBFTNAME = $(LIBFTDIR)/libft.a
LIBS =  -L$(LIBFTDIR) -lft
EXT_LIBS = -lreadline

<<<<<<<< HEAD:tmp/Makefile

.PHONY: all re clean fclean makelibft
========
SRCS = main/minishell.c main/init_shell.c main/redirections.c main/split_while_skipping_quotes.c

BUILTIN = builtins/builtin.c builtins/ft_echo.c

ERR = err/error_msg.c

SRC = $(SRCS) $(ERR) $(BUILTIN)

OBJDIR = obj/
OBJS = $(addprefix $(OBJDIR), $(SRC:.c=.o))
>>>>>>>> main:Makefile

all: makelibft $(NAME)

$(OBJDIR)%.o: src/%.c $(INCLDS)
	mkdir -p $(dir $@)
	@$(CC) -c $(CFLAGS) $(DEPFLAGS) $< -o $@

$(NAME): $(LIBFTNAME) $(OBJS)
	cc $(CFLAGS) $(OBJS) $(LIBS) $(EXT_LIBS) -o $(NAME)

makelibft:
	cd $(LIBFTDIR) && make

<<<<<<<< HEAD:tmp/Makefile
$(OBJS): obj/%.o: src/%.c $(INCLDS)
	cc $(CFLAGS) -c $< -o $@

========
>>>>>>>> main:Makefile
clean:
	rm -f $(OBJS)
	cd $(LIBFTDIR) && make clean

fclean: clean
	rm -f $(NAME)
	rm -rf $(OBJDIR)
	cd $(LIBFTDIR) && make fclean

re: fclean all

.PHONY: all re clean fclean makelibft