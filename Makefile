SRCS = game.cpp
# CPPFLAGS = -Wall -Werror -Wextra
SANITIZE = -fsanitize=address
CC = g++ -std=c++2a

all :	
	${CC} ${CPPFLAGS} ${SRCS} -o mcts
flag:
	${CC} ${CPPFLAGS} ${SANITIZE} -g3 ${SRCS} -o mcts
clean :
	rm -rf mcts
re: clean all
