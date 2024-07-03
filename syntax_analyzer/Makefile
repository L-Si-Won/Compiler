NAME = syntax_analyzer

CC = cc

SOURCE = \
	 y.tab.c \
	 lex.yy.c \

all : $(NAME)

$(NAME) :
	yacc -d kim.y
	lex kim.l
	cc $(SOURCE) print.c -o $(NAME) -w

clean :
	rm -f $(SOURCE) y.tab.h

fclean : clean
	rm -f $(NAME)

re : fclean all
