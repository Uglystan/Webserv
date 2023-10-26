NAME = webserv
CC = c++
FLAGS = -Wall -Wextra -Werror -g3
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
	srcs/server/selectGoodServ.cpp\
	srcs/server/handleError.cpp\
	srcs/server/chunkedReq.cpp\
	srcs/parsingconf/parsingConf.cpp\
	srcs/parsingconf/cutStringConf.cpp\
	srcs/parsingconf/fillServConf.cpp\
	srcs/parsingconf/getLocation.cpp\
	srcs/parsingconf/checkData.cpp\
	srcs/http/response/response.cpp\
	srcs/http/CGIparsing/CGIparsing.cpp\
	srcs/http/header_parsing/header_parsing.cpp\
	srcs/http/cgi_handler/cgi_handler.cpp\

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