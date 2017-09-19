## No Header
## 31/07/2017 - 13:54:05
##

NAME=		yellow
VERSION=	.1.0

SRC=		./src/
INC=		./inc/
OBJ=		./obj/

#FILES=		$(shell tree -fin $(SRC) | grep \.c$)
FILES=		main.c		\
		init.c		\
		destroy.c	\
		utils.c		\
		socket.c	\
		eth_process.c	\
		ip_process.c	\
		loop.c		\
		tmp.c		\
		display.c	\
		pcap.c

ifeq ($(GEOLOC),1)
	FILES+= geoloc.c
endif

OBJS=		$(FILES:%.c=$(OBJ)%.o)

DFLAGS=		-DDBG -g
CFLAGS=		-W -Wall -Wextra -Werror $(DFLAGS) -I$(INC)

LDFLAGS=	-lncurses -lmenu -lpcap

ifeq ($(GEOLOC),1)
	LDFLAGS+=	-lGeoIP
endif
CC=		gcc

all:		$(NAME)

$(OBJ)%.o:	$(SRC)%.c
		@$(CC) $(CFLAGS) -c -o $@ $<
		@echo -e "[OK]\t$@"

$(NAME):	$(OBJS)
		@$(CC) -o $(NAME) $(CFLAGS) $(OBJS) $(LDFLAGS)
		@echo -e "[DONE]\t$(NAME)"

clean:
		@rm -vf $(OBJS)

fclean:		clean
		@rm -vf $(NAME)

re:		fclean all

.PHONY: all clean fclean re
