NAME		=	IRC
INC			= 	ft_irc.h
CXX			=	c++
CXXFLAGS	=	#-Wall -Wextra -Werror -std=c++98


# AR		=	ar rc
RM		=	rm -f

SRC		=	main.cpp server.cpp client.cpp

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
