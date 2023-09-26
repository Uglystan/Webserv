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
	srcs/server/sizeMessageRecv.cpp\
	srcs/server/readMessage.cpp\
	srcs/utils/gnl.cpp\
	
SCRIPT_SRC = srcs/cgi/cgi.cpp

OBJ = $(SRC:.cpp=.o)
SCRIPT_OBJ = $(SCRIPT_SRC:.cpp=.o)

%.o : %.cpp
	@$(CC) $(FLAGS) $(C98) -c $< -o $@

all : $(NAME)

$(NAME): $(OBJ) $(SCRIPT_OBJS)
	@echo "$(BLUE)$(BOLD)Compiling $(NAME)...$(RESET)"
	@$(CC) $(FLAGS) $(C98) $(OBJ) -o $(NAME)
	@echo "$(GREEN)$(BOLD)$(NAME) compiled!$(RESET)"

a.out: $(SCRIPT_OBJ)
	@echo "$(BLUE)$(BOLD)Compiling a.out...$(RESET)"
	@$(CC) $(FLAGS) $(C98) $(SCRIPT_OBJ) -o srcs/cgi/a.out
	@echo "$(GREEN)$(BOLD)a.out compiled!$(RESET)"

all: $(NAME) a.out

clean:
	@echo "$(BLUE)$(BOLD)Cleaning object files...$(RESET)"
	@$(RM) $(OBJ) $(SCRIPT_OBJ)

fclean: clean
	@echo "$(BLUE)$(BOLD)Cleaning $(NAME)...$(RESET)"
	@$(RM) $(NAME)

	@echo "$(BLUE)$(BOLD)Cleaning a.out...$(RESET)"
	@$(RM) srcs/cgi/a.out

re: fclean all

.PHONY: init all clean fclean re