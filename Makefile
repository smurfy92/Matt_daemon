# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jtranchi <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/09/06 16:48:55 by jtranchi          #+#    #+#              #
#    Updated: 2018/09/06 16:48:57 by jtranchi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC = daemon.cpp
SRC += tintin_logger.cpp

NAME = Matt_daemon

SRCDIR = srcs/
OBJDIR = objs/
OBJ = $(addprefix $(OBJDIR), $(SRC:.cpp=.o))

CG = \033[92m
CY =  \033[93m
CE = \033[0m
CB = \033[34m
INC = -I ./includes/

all: start $(NAME)


$(NAME): $(OBJ)
	@g++ -o $(NAME) $(OBJ) $(FLAG) $(INC) $(LIB);
	@echo "\033[K$(CY)[MATT_DAEMON] :$(CE) $(CG)Compiling Matt_daemon$(CE)";


start:
	@mkdir -p $(OBJDIR)
	@echo "\n";
	@echo "			$(CG)      :::::::::: :::::::::::           ::::::::   ::::::::  :::   $(CE)";
	@echo "			$(CG)     :+:            :+:              :+:    :+: :+:    :+: :+:    $(CE)";
	@echo "			$(CG)    +:+            +:+              +:+        +:+        +:+     $(CE)";
	@echo "			$(CG)   :#::+::#       +#+              +#++:++#++ +#++:++#++ +#+      $(CE)";
	@echo "			$(CG)  +#+            +#+                     +#+        +#+ +#+       $(CE)";
	@echo "			$(CG) #+#            #+#              #+#    #+# #+#    #+# #+#        $(CE)";
	@echo "			$(CG)###            ###    ########## ########   ########  ##########  $(CE)";
	@echo "\n";

$(OBJ): $(OBJDIR)%.o: $(SRCDIR)%.cpp
	@echo "\033[K$(CY)[MATT_DAEMON] :$(CE) $(CG)Compiling $<$(CE)";
	@g++ $(FLAG) -c $< -o $@ $(INC)

clean: start
	@echo "$(CY)[MATT_DAEMON] :$(CE) $(CG)Cleaning Matt_daemon objects$(CE)";
	@/bin/rm -rf $(OBJ);

fclean: start clean
	@echo "\033[K$(CY)[MATT_DAEMON] :$(CE) $(CG)Cleaning binairies ...$(CE)";
	@/bin/rm -f $(NAME);

re: fclean all

.PHONY: all clean fclean re
