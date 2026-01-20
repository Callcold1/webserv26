# USEFUL IF THE MAKEFILE IS NOT IN THE PROJECT ROOT FOLDER
PROJECT_ROOT := $(realpath ../../../../..)

NAME        := versionTest

#CHANGE ALL CXX TO CPP FOR C PROGRAMS
CXX         := g++
CXXFLAGS    := -Wall -Werror -Wextra -std=c++98 -g

INCLUDES    := -I$(PROJECT_ROOT)/Request/includes \
               -I$(PROJECT_ROOT)/Config/includes \
			   -I ./

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
               tests/testPrograms/RequestParserTests/RequestLineParserTests/versionParsing/test_parse_version.cpp

OBJS := $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCS))
DEPS := $(patsubst %.cpp, $(OBJDIR)/%.d, $(SRCS))

all: $(NAME)

# LINKING OBJECT FILES TO MAKE EXECUTABLE
$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) -o $(NAME)
	@$(foreach obj,$(OBJS),echo "Linking object file" $(obj);)
	@echo "$(NAME) created"

# COMPILING/RECOMPILING OBJECT FILES IF ANY DEPENDENCIES CHANGE
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	@echo "Compiling Object File $< "
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c -MMD -MP $< -o $@LINKING OBJECT FILES TO MAKE EXECUTABLE

-include $(DEPS)

clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
