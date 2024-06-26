##########################################################################################################################

# Target executable name
NAME = webserv

# Compiler and compilation flags
CC = g++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g #-fsanitize=address -fsanitize=leak

# Directory structure
OBJ_DIR = objs/
SRC = $(wildcard srcs/*.cpp)
OBJS = $(patsubst srcs/%, $(OBJ_DIR)%, $(SRC:.cpp=.o))

CONFS = $(DEFAULT_CONF) $(MULTIPLE_PORTS_CONF)
DEFAULT_CONF = confs/default.conf
MULTIPLE_PORTS_CONF = confs/multiple_ports.conf
##########################################################################################################################

# General rules
all: $(NAME) $(CONFS)

clean:
	@rm -rf $(OBJS)
	@rm -rf $(OBJ_DIR)
	@echo "$(RED)Removing:$(DEFAULT) All objects from $(NAME)"

fclean: clean
	@rm -rf $(NAME)
	@echo "$(RED)Removing:$(DEFAULT) Program $(NAME)"
	@rm -rf $(CONFS)
	@echo "$(RED)Removing:$(DEFAULT) $(CONFS)"

re: fclean all

##########################################################################################################################

# Build rules
$(NAME): $(OBJ_DIR) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(MAGENTA)Program $(NAME) created successfully.$(DEFAULT)"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@echo "$(GREEN)Creating:$(DEFAULT) $(OBJ_DIR) directory"

$(OBJ_DIR)%.o: srcs/%.cpp | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREEN)Compiling:$(DEFAULT) $<"

$(DEFAULT_CONF):
	@sed "s#\__PWD__#$(shell pwd)#g" confs/templates/template_default.conf > confs/default.conf
	@echo "$(GREEN)Creating:$(DEFAULT) default.conf"

$(MULTIPLE_PORTS_CONF):
	@sed "s#\__PWD__#$(shell pwd)#g" confs/templates/template_multiple_ports.conf > confs/multiple_ports.conf
	@echo "$(GREEN)Creating:$(DEFAULT) multiple_ports.conf"

##########################################################################################################################

# Personal use rules
DATETIME := $(shell date +%Y-%m-%d' '%H:%M:%S)
USER := $(shell whoami)
GITIGNORE = .gitignore

$(GITIGNORE):
	@echo ".*\n*.out\n*.o\n*.a\n*.dSYM\n" > .gitignore
	@echo "$(GREEN)Creating:$(DEFAULT) Gitignore"

gitignore: $(GITIGNORE)

git: clean $(GITIGNORE)
	@git add *
	@echo "$(BOLD)$(YELLOW)Git:$(WHITE) Adding all archives.$(DEFAULT)"
	@git commit -m "[$(DATETIME)] - Little changes by $(USER)"
	@echo "$(BOLD)$(CYAN)Git:$(WHITE) Commit this changes with \"[$(DATETIME)] - Little changes by $(USER)\".$(DEFAULT)"
	@git push
	@echo "$(BOLD)$(GREEN)Git:$(WHITE) Pushing all changes.$(DEFAULT)"

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

.PHONY: all clean fclean re git gitignore


