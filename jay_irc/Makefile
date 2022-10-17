NOCOLOR		= \033[0m
RED			= \033[0;31m
GREEN		= \033[0;32m
ORANGE		= \033[0;33m
BLUE		= \033[0;34m
PURPLE		= \033[0;35m
CYAN		= \033[0;36m
LIGHTGRAY	= \033[0;37m
DARKGRAY	= \033[1;30m
LIGHTRED	= \033[1;31m
LIGHTGREEN	= \033[1;32m
YELLOW		= \033[1;33m
LIGHTBLUE	= \033[1;34m
LIGHTPURPLE	= \033[1;35m
LIGHTCYAN	= \033[1;36m
WHITE		= \033[1;37m

ECHO		= echo

NAME		=	ircserv
CXX			=	c++
CXXFLAGS	=   -std=c++98 -Wall -Wextra -Werror
RM			=	rm
RMFLAGS		=	-rf

SRC_DIR		=	./src
INC_DIR		=	./inc
OBJ_DIR		=	./obj

INCS		=	$(wildcard $(INC_DIR)/*.hpp)
SRCS		=	$(wildcard $(SRC_DIR)/*.cpp)
OBJS		=	$(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))

vpath %.cpp $(SRC_DIR) $(INC_DIR)

all : $(NAME)

$(OBJ_DIR) :
	@mkdir $(OBJ_DIR)

$(OBJ_DIR)/%.o : %.cpp $(INCS) | $(OBJ_DIR)
	@$(CXX) $(CXXFLAGS) -I $(INC_DIR) -c $< -o $@
	@$(ECHO) "Compiled $(ORANGE)" $< " $(GREEN)Successfully$(NOCOLOR)"

$(NAME) : $(OBJS) $(INCS)
	@$(CXX) $(CXXFLAGS) -I $(INC_DIR) $(OBJS) -o $@
	@$(ECHO) "Maked $(BLUE)$(NAME)$(NOCOLOR) $(GREEN)Successfully$(NOCOLOR)"

clean :
	@$(RM) $(RMFLAGS) $(OBJ_DIR)
	@$(ECHO) "Clean up $(RED)$(NAME)$(NOCOLOR)"

fclean : clean
	@$(RM) $(RMFLAGS) $(NAME)
	@$(ECHO) "Removed $(RED)$(NAME)$(NOCOLOR)"

re : fclean $(NAME)

.PHONY : all clean fclean re
