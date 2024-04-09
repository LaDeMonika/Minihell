CFLAGS = -Wall -Werror -Wextra -pthread -g
SRCS = main.c
INCLDS = philo.h
OBJS = $(SRCS:%.c=%.o)
NAME = philo

.PHONY: all re clean fclean

all: $(NAME)

$(NAME): $(OBJS)
	cc $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJS): %.o: %.c $(INCLDS)
	cc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean make