NAME		=	ircserv
CXX			=	c++
CXXFLAGS	=	-g -std=c++98
RM			=	rm
RMFLAGS		=	-rf

SRC_DIR		=	./src
INC_DIR		=	./inc
OBJ_DIR		=	./obj

INCS		=	$(wildcard $(INC_DIR)/*.hpp)
SRCS		=	$(SRC_DIR)/test.cpp
OBJS		=	$(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))

vpath %.cpp $(SRC_DIR) $(INC_DIR)

all : $(NAME)

$(OBJ_DIR) :
	@mkdir $(OBJ_DIR)

$(OBJ_DIR)/%.o : %.cpp $(INCS) | $(OBJ_DIR)
	@printf "$(CXX) $(CXXFLAGS) -I $(INC_DIR) -c $< -o $@\n"
	$(CXX) $(CXXFLAGS) -I $(INC_DIR) -c $< -o $@

$(NAME) : $(OBJS) $(INCS)
	@printf "$(CXX) $(CXXFLAGS) -I $(INC_DIR) $(OBJS) -o $@\n"
	$(CXX) $(CXXFLAGS) -I $(INC_DIR) $(OBJS) -o $@

clean :
	$(RM) $(RMFLAGS) $(OBJS)

fclean : clean
	$(RM) $(NAME)

re :
	make fclean
	make all

.PHONY : all clean fclean re