CFLAGS = -Wall -Werror -Wextra -g
SRCS = src/main.c
INCLDS = inc/minishell.h
OBJS = $(SRCS:src/%.c=obj/%.o)
NAME = minishell
LIBFTDIR = ./libft
LIBFTNAME = $(LIBFTDIR)/libft.a
LIBS =  -L$(LIBFTDIR) -lft
EXT_LIBS = -lreadline


.PHONY: all re clean fclean makelibft

all: makelibft $(NAME)

$(NAME): $(LIBFTNAME) $(OBJS)
	cc $(CFLAGS) $(OBJS) $(LIBS) $(EXT_LIBS) -o $(NAME)

makelibft:
	cd $(LIBFTDIR) && make

$(OBJS): obj/%.o: src/%.c $(INCLDS)
	cc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	cd $(LIBFTDIR) && make clean

fclean: clean
	rm -f $(NAME)
	cd $(LIBFTDIR) && make fclean

re: fclean all
