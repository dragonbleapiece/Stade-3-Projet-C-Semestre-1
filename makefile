ifdef COMSPEC
 MV ?= move
 RM ?= del
else
 MV ?= mv -f
 RM ?= rm -f
endif

CC=gcc
CFLAGS=-W -Wall -ansi -pedantic
LDFLAGS=
EXEC=jeu
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

jeu: $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)

main.o: functions.h structures.h

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@$(RM) *.o

mrproper: clean
	@$(RM) $(EXEC)
