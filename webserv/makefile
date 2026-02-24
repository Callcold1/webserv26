# USEFUL IF THE MAKEFILE IS NOT IN THE PROJECT ROOT FOLDER
PROJECT_ROOT := $(realpath ./)

NAME        := webserv

CXX         := g++
CXXFLAGS    := -Wall -Werror -Wextra -std=c++98 -fsanitize=address -g

INCLUDES    :=	-I $(PROJECT_ROOT)/includes \
				-I $(PROJECT_ROOT)/includes/response \
 				-I$(PROJECT_ROOT)/includes/config \
 				-I$(PROJECT_ROOT)/includes/request
#  				-I$(PROJECT_ROOT)/Request/includes \

OBJDIR      := objs
SRCDIR      := $(PROJECT_ROOT)/srcs

SRCS        := main.cpp \
				Server.cpp \
				response/Response.cpp \
				response/ErrorResponse.cpp \
				response/RediResponse.cpp \
				response/StaticResponse.cpp \
				response/CgiResponse.cpp \
				utilityFunctions.cpp \
				Log.cpp \
				config/Config.cpp \
				config/CParser.cpp \
				config/CServer.cpp \
				config/CLocation.cpp \
				config/Parser.cpp \
				config/Tokeniser.cpp \
				request/Request.cpp \
				request/RequestParser.cpp \
				request/RequestValidator.cpp \
				Client.cpp

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
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c -MMD -MP $< -o $@

-include $(DEPS)

clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
