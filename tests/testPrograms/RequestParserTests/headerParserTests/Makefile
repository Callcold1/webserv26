NAME 		=	headerTest

CPP			=	g++
#CPP			=	c++
#CPPFLAGS	=	-Wall -Werror -Wextra -std=c++98 -g -O0
CPPFLAGS	=	-Wall -Werror -Wextra -std=c++98 -g

INCLUDES	=	./

OBJDIR 		= 	objs

SRCDIR		=	../../..

SRCS		=	Request/Request.cpp \
				Request/RequestParser.cpp \
				Request/RequestValidator.cpp \
				Request/utility_functions.cpp \
				Request/TestClasses/testUtils.cpp \
				Config/Config.cpp \
				Config/Route.cpp \
				Config/ConfigParser.cpp \
				tests/testPrograms/headerParsing/test_parse_headers.cpp \

OBJS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCS))
DEPS = $(patsubst %.cpp, $(OBJDIR)/%.d, $(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	@$(CPP) $(CPPFLAGS) $(OBJS) -I $(INCLUDES) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CPP) $(CPPFLAGS) -c -MMD -MP $< -o $@

-include $(DEPS)

clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all
.PHONY: all clean fclean re