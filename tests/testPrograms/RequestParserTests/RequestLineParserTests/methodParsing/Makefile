PROJECT_ROOT := $(realpath ../../../../..)

NAME        := methodTest
CXX         := g++
CXXFLAGS    := -Wall -Werror -Wextra -std=c++98 -g

INCLUDES    := -I$(PROJECT_ROOT)/Request/includes \
               -I$(PROJECT_ROOT)/Config/includes \
			   -I ./
#-I$(PROJECT_ROOT)/tests/testPrograms/testMain

OBJDIR      := objs
SRCDIR      := $(PROJECT_ROOT)

SRCS        := Request/Request.cpp \
               Request/RequestParser.cpp \
               Request/RequestValidator.cpp \
               Request/utility_functions.cpp \
               Request/TestClasses/testUtils.cpp \
               Config/Config.cpp \
               Config/Route.cpp \
               Config/ConfigParser.cpp \
			   tests/testPrograms/RequestParserTests/RequestLineParserTests/methodParsing/test_parse_method.cpp
#tests/testPrograms/testMain/tests.cpp
               
OBJS := $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCS))
DEPS := $(patsubst %.cpp, $(OBJDIR)/%.d, $(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c -MMD -MP $< -o $@

-include $(DEPS)

clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
