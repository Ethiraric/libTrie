##
## Makefile for  in /home/sabour_f/rendu/libTrie
##
## Made by Florian SABOURIN
## Login   <sabour_f@epitech.net>
##
## Started on  Wed Jun 17 19:39:33 2015 Florian SABOURIN
## Last update Wed Jun 17 23:14:18 2015 Florian SABOURIN
##


NAME		=	libTrie.a
CC		=	gcc
AR		=	ar rcs
override CFLAGS	+=	-W -Wall -Iinclude -D_GNU_SOURCE -O3
LDFLAGS		=
RM		=	@rm -vf
MAKE		+=	--no-print-directory

SRC		=	src/trie.c

OBJ		=	$(SRC:.c=.o)

$(NAME):	$(OBJ)
	$(AR) $(NAME) $(OBJ)

all:	$(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re:	fclean all

.PHONY: all clean fclean re
