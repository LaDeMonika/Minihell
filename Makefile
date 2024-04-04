NAME = minishell
CFLAGS = -Wall -Werror -Wextra -g
INCLDS = inc/minishell.h
OBJS = $(SRCS:%.c=%.o)
LIBFTDIR = ./libft
LIBFTNAME = $(LIBFTDIR)/libft.a
LIBS =  -L$(LIBFTDIR) -lft
EXT_LIBS = -lreadline

SRC_DIR = src/minishell.c

SRC_ERR = err/error_msg.c

SRCS = $(SRC_DIR) $(SRC_ERR)

all: makelibft $(NAME)

$(NAME): $(LIBFTNAME) $(OBJS)
	cc $(CFLAGS) $(OBJS) $(LIBS) $(EXT_LIBS)  -o $(NAME)

makelibft:
	cd $(LIBFTDIR) && make

$(OBJS): %.o: %.c $(INCLDS)
	cc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	cd $(LIBFTDIR) && make clean

fclean: clean
	rm -f $(NAME)
	cd $(LIBFTDIR) && make fclean

re: fclean all

.PHONY: all re clean fclean makelibft