##########################################################################################################################

# Target executable name
NAME = webserv

# Compiler and compilation flags
CC = g++
CFLAGS = -Wall -Werror -Wextra -std=c++98 -fsanitize=address

# Directory structure
OBJ_DIR = objs/
SRC = $(wildcard srcs/*.cpp)
OBJS = $(patsubst srcs/%, $(OBJ_DIR)%, $(SRC:.cpp=.o))

##########################################################################################################################

# General rules
all: $(NAME)

clean:
	@rm -rf $(OBJS)
	@rm -rf $(OBJ_DIR)
	@echo "$(RED)Removing:$(DEFAULT) All objects from $(NAME)."

fclean: clean
	@rm -rf $(NAME)
	@echo "$(RED)Removing:$(DEFAULT) Program $(NAME)."

re: fclean all

##########################################################################################################################

# Build rules
$(NAME): $(OBJ_DIR) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(MAGENTA)Program $(NAME) created successfully.$(DEFAULT)"

$(OBJ_DIR):
	@echo "\n$(BLUE)"
	@echo "██╗    ██╗███████╗██████╗ ███████╗███████╗██████╗ ██╗   ██╗"
	@echo "██║    ██║██╔════╝██╔══██╗██╔════╝██╔════╝██╔══██╗██║   ██║"
	@echo "██║ █╗ ██║█████╗  ██████╔╝███████╗█████╗  ██████╔╝██║   ██║"
	@echo "██║███╗██║██╔══╝  ██╔══██╗╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝"
	@echo "╚███╔███╔╝███████╗██████╔╝███████║███████╗██║  ██║ ╚████╔╝ "
	@echo " ╚══╝╚══╝ ╚══════╝╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝  "
	@echo "\n$(CYAN)"
	@echo "---------------------  Created by -------------------------"
	@echo "------------  alvgomez && fgomez-d && javiersa ------------"
	@echo "$(DEFAULT)"
	@mkdir -p $(OBJ_DIR)
	@echo "$(GREEN)Creating:$(DEFAULT) $(OBJ_DIR) directory."

$(OBJ_DIR)%.o: srcs/%.cpp | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREEN)Compiling:$(DEFAULT) $<"

##########################################################################################################################

# Personal use rules
DATETIME := $(shell date +%Y-%m-%d' '%H:%M:%S)
USER := $(shell whoami)
GITIGNORE = .gitignore

$(GITIGNORE):
	@echo ".*\n*.out\n*.o\n*.a\n*.dSYM\n!.gitignore" > .gitignore
	@echo "$(GREEN)Creating:$(DEFAULT) Gitignore."

git: clean $(GITIGNORE)
	@git add *
	@echo "$(BOLD)$(YELLOW)Git:$(WHITE) Adding all archives.$(DEFAULT)"
	@git commit -m "[$(DATETIME)] - Little changes by $(USER)"
	@echo "$(BOLD)$(CYAN)Git:$(WHITE) Commit this changes with \"[$(DATETIME)] - Little changes by $(USER)\".$(DEFAULT)"
	@git push
	@echo "$(BOLD)$(GREEN)Git:$(WHITE) Pushing all changes.$(DEFAULT)"

test_response: $(OBJS)
	@$(CC) $(CFLAGS) -c srcs/test_response.cpp -o objs/test_response.o
	@$(CC) $(CFLAGS) $(OBJS) -o test_response

# Color codes for improved readability
BOLD    := \033[1m
BLACK   := \033[30;1m
RED     := \033[31;1m
GREEN   := \033[32;1m
YELLOW  := \033[33;1m
BLUE    := \033[34;1m
MAGENTA := \033[35;1m
CYAN    := \033[36;1m
WHITE   := \033[37;1m
DEFAULT := \033[0m

.PHONY: all clean fclean re git


