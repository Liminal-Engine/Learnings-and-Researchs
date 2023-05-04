CXX  =   g++
INCLUDES    = -I include
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
CXXFLAGS  =   $(INCLUDES) $(LDFLAGS) -std=c++23 -O2 -g -g3 #-Wall -Wextra #-Werror
EXEC    =   vulkanEngine
SRC =   $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*/**/*.cpp)
OBJ =   $(SRC:.cpp=.o)
.PHONY: clean fclean all re

END         =   \033[0m
BOLD        =   \033[1m
GREY        =   \033[30m
RED         =   \033[31m
GREEN       =   \033[32m
BLINK       =   \033[32;5m
YELLOW      =   \033[33m
BLUE        =   \033[34m
PURPLE      =   \033[35m
CYAN        =   \033[36m
WHITE       =   \033[37m

all: $(EXEC)
	@echo "$(END)"

start:
	@echo "$(BOLD)$(CYAN)\n\n\t-----STARTING COMPILATION-----\n\n$(YELLOW)"

$(EXEC): start $(OBJ)
	@$(CXX) $(SRC) $(CXXFLAGS) -o $(EXEC) &&\
	echo "$(BOLD)$(YELLOW)\n\n\t-----COMPILED SUCCESSFULLY-----" && \
	echo "\t-----BINARY NAME IS : $(EXEC)-----\n"

clean:
	@rm -rf $(OBJ)
	@echo "$(BOLD)$(GREEN)\t-----WORKSPACE CLEANED-----"

fclean: clean
	@rm -rf $(EXEC)
	@echo "$(BOLD)$(GREEN)\t-----WORKSPACE FCLEANED-----"

re: fclean all

