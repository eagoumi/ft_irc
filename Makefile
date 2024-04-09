NAME		=	ircserv

CC			=	c++

CFLAGS		=	-Wall -Wextra -std=c++98 -Wshadow -fsanitize=address -g

DEBUG		=	DEBUG=true

# directories
SRC_DIR		=	./
OBJ_DIR		=	obj/
INC_DIR		=	./

# controll codes
RESET		=	\033[0m
GREEN		=	\033[32m
YELLOW		=	\033[33m
BLUE		=	\033[34m
RED			=	\033[31m
UP			=	\033[A
CUT			=	\033[K

#source files
SRC_FILES	=	Server/server.cpp \
                Server/main.cpp \
				Database/database.cpp Database/debug.cpp \
				Channels/channel.cpp \
				Users/user.cpp \
				Commands/Commands.cpp Commands/kick.cpp \
				Commands/join.cpp Commands/invite.cpp \
				Commands/topic.cpp Commands/mode.cpp \
				Commands/whois.cpp Commands/location.cpp \
				Commands/logtime.cpp \
				Commands/PRIVMSG.cpp \
				Commands/Part.cpp \
				Commands/authentication.cpp \
				Logger/logger.cpp

INC_FILES	=	Server/server.hpp \
				Database/database.hpp \
				Logger/logger.hpp \
				Channels/channel.hpp \
				Users/user.hpp \
				error_request.hpp \
				Commands/Commands.hpp


OBJ_FILES	=	$(SRC_FILES:.cpp=.o)

#paths
SRC			=	$(addprefix $(SRC_DIR), $(SRC_FILES))
OBJ			=	$(addprefix $(OBJ_DIR), $(OBJ_FILES))

#all rule
all: $(NAME)
	@printf "\n"
	@printf "$(GREEN)██╗██████╗  █████╗ ███████╗██╗      █████╗ ██╗    ██╗███╗   ██╗    ███╗   ██╗    ██╗██████╗  ██████╗\n$(RESET)"
	@printf "$(GREEN)██║██╔══██╗██╔══██╗╚════██║██║     ██╔══██╗██║    ██║████╗  ██║    ████╗  ██║    ██║██╔══██╗██╔════╝\n$(RESET)"
	@printf "$(GREEN)██║██████╔╝███████║    ██╔╝██║     ███████║██║ █╗ ██║██╔██╗ ██║    ██╔██╗ ██║    ██║██████╔╝██║     \n$(RESET)"
	@printf "$(GREEN)██║██╔══██╗██╔══██║   ██╔╝ ██║     ██╔══██║██║███╗██║██║╚██╗██║    ██║╚██╗██║    ██║██╔══██╗██║     \n$(RESET)"
	@printf "$(GREEN)██║██████╔╝██║  ██║   ██║  ███████╗██║  ██║╚███╔███╔╝██║ ╚████║    ██║ ╚████║    ██║██║  ██║╚██████╗\n$(RESET)"
	@printf "$(GREEN)╚═╝╚═════╝ ╚═╝  ╚═╝   ╚═╝  ╚══════╝╚═╝  ╚═╝ ╚══╝╚══╝ ╚═╝  ╚═══╝    ╚═╝  ╚═══╝    ╚═╝╚═╝  ╚═╝ ╚═════╝\n$(RESET)"
	@printf "\n"

#compile the executable
$(NAME): $(OBJ) $(INC_FILES)
	@echo "$(YELLOW)Compiling [$(NAME)]...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "$(GREEN)Finished [$(NAME)]$(RESET)"

#compile objects
$(OBJ_DIR)%.o:$(SRC_DIR)%.cpp $(INC_FILES)
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)Compiling [$@]...$(RESET)"
	@$(CC) $(CFLAGS) -I $(INC_DIR) -o $@ -c $< -D$(DEBUG)
	@printf "$(UP)$(CUT)"
	@echo "$(GREEN)Finished [$@]$(RESET)"
	@printf "$(UP)$(CUT)"

#clean rule
clean:
	@if [ -d "$(OBJ_DIR)" ]; then \
	rm -rf $(OBJ_DIR); \
	echo "$(BLUE)Deleting all objects from /FT_IRC...$(RESET)"; else \
	echo "No objects to remove from /FT_IRC."; \
	fi;

#fclean rule
fclean: clean
	@if [ -f "$(NAME)" ]; then \
	rm -f $(NAME); \
	echo "$(BLUE)Deleting $(NAME) from /FT_IRC...$(RESET)"; else \
	echo "No Executable to remove from /FT_IRC."; \
	fi;

#re rule
re: fclean all

#phony
.PHONY: all clean fclean re    