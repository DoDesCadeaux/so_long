NAME = solong.a

SRCS =		solong.c\
			GNL/get_next_line.c\
			GNL/get_next_line_utils.c\

OBJ =		$(SRCS:.c=.o)

RM = rm -f

CC = gcc

CFLAGS = -Wall -Wextra -Werror -Imlx

${NAME}:	${OBJ}
	make -C libft
	${CC} ${OBJ} -Lmlx -lmlx -framework OpenGL -framework AppKit -o ${NAME} libft/libft.a

all:	${NAME}

clean:
		${RM}	${OBJ}

fclean:		clean
	${RM}	${NAME}

re:		fclean	all

.PHONY: all clean fclean re