NAME = minishell

CFLAGS = -Wall -Werror -Wextra -g
INCLDS = inc/minishell.h
LIBFTDIR = ./libft
LIBFTNAME = $(LIBFTDIR)/libft.a
LIBS =  -L$(LIBFTDIR) -lft
EXT_LIBS = -lreadline

BLA = main/minishell.c

ERR = err/error_msg.c

SRC = $(BLA) $(ERR)

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