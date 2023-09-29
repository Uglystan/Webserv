NAME = webserv
CC = c++
FLAGS = -Wall -Wextra -Werror
C98 = -std=c++98
RM = rm -rf

GREEN=\033[33;32m
BLUE=\033[0;34m
BOLD=\033[1m
RESET=\033[0m

SRC =	srcs/server/serv.cpp\
	srcs/server/manageEpoll.cpp\
	srcs/server/initSockServ.cpp\
	srcs/server/manageSockClient.cpp\
	srcs/server/recvMultipart.cpp\
	srcs/parsingconf/parsingConf.cpp\
	srcs/parsingconf/cutStringConf.cpp\
	srcs/parsingconf/fillServConf.cpp\
	srcs/parsingconf/getLocation.cpp\
	srcs/parsingconf/checkData.cpp\
	srcs/utils/gnl.cpp\
	srcs/response/response.cpp\
	srcs/cgi_handler/cgi_handler.cpp\
	srcs/httpparsing/httpparsing.cpp\

OBJ = $(SRC:.cpp=.o)

%.o : %.cpp
	@$(CC) $(FLAGS) $(C98) -c $< -o $@

all : $(NAME)

$(NAME): $(OBJ)
	@echo "$(BLUE)$(BOLD)Compiling $(NAME)...$(RESET)"
	@$(CC) $(FLAGS) $(C98) $(OBJ) -o $(NAME)
	@echo "$(GREEN)$(BOLD)$(NAME) compiled!$(RESET)"

all: $(NAME)

clean:
	@echo "$(BLUE)$(BOLD)Cleaning object files...$(RESET)"
	@$(RM) $(OBJ)

fclean: clean
	@echo "$(BLUE)$(BOLD)Cleaning $(NAME)...$(RESET)"
	@$(RM) $(NAME)

re: fclean all

.PHONY: init all clean fclean re