NAME = interpreter

CC = cc

SOURCE = \
	 y.tab.c \
	 lex.yy.c \

all : $(NAME)

$(NAME) :
	yacc -d interp.y
	lex interp.l
	cc $(SOURCE) interp.c lib.c -o $(NAME) -w -m32

clean :
	rm -f $(SOURCE) y.tab.h

fclean : clean
	rm -f $(NAME)

re : fclean all
