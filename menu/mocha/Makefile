NAME = mocha
CC = cc
AR = ar
SRC = $(NAME).c
OBJ = $(SRC:%.c=%.o)

INCLUDE = -I. -Iexternal

CFLAGS = -Wall -std=c99
LFLAGS = -L. -l$(NAME) -lm -lpthread -ldl

LIBNAME = lib$(NAME).a

.PHONY: build clean
.SECONDARY: $(OBJ) $(LIBNAME)

build: $(LIBNAME)

hello: $(LIBNAME)
	$(CC) examples/hello/main.c -o hello $(INCLUDE) $(CFLAGS) $(LFLAGS)

%.a: $(OBJ)
	$(AR) rcs $@ $(OBJ)

%.o: %.c
	$(CC) -c $< -o $@ $(INCLUDE) $(CFLAGS)

clean:
	rm -f $(OBJ)
	rm -f $(LIBNAME)
	rm -f hello
