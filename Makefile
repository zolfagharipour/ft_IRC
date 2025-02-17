NAME		=	ircserv
INC			= 	ft_irc.h
CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98

RM		=	rm -f

SRC		=	$(wildcard *.cpp) 

OBJS	=	$(SRC:.cpp=.o)

.PHONY:		all clean fclean re

all: ${NAME}

${NAME}: ${OBJS}
	${CXX} ${CXXFLAGS} -o ${NAME} $^

%.o: %.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@

clean:
			$(RM) $(OBJS)

fclean: 	clean
			$(RM) $(NAME)

re:			clean all

test: ${NAME}
	clear && valgrind -s --leak-check=full --leak-check=full --show-leak-kinds=all --track-fds=yes ./ircserv 6667 00


