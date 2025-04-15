NAME = pipex

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC = pipex.c utils.c libft/ft_putstr_fd.c libft/ft_split.c libft/ft_strdup.c libft/ft_strjoin.c libft/ft_strlen.c libft/ft_strncmp.c libft/ft_substr.c
OBJ = $(SRC:.c=.o)

INCLUDE = pipex.h

RM = rm -f

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all
