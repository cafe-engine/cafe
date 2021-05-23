CC = cc
SRC = tea.c
OBJ = $(SRC:%.c=%.o)

INCLUDE = -I. -Iexternal

CFLAGS = -Wall -std=c99 `sdl2-config --cflags`
LFLAGS = -L. -ltea -lm `sdl2-config --libs`

LIBNAME = libtea.a

.PHONY: build clean
.SECONDARY: $(OBJ) $(LIBNAME)

build: $(LIBNAME)

hello: $(LIBNAME) examples/hello/main.o
	$(CC) examples/hello/main.o -o hello $(INCLUDE) $(CFLAGS) $(LFLAGS)

%.a: $(OBJ)
	$(AR) rcs $@ $(OBJ)

%.o: %.c
	$(CC) -c $< -o $@ $(INCLUDE) $(CFLAGS)

clean:
	rm -f $(OBJ)
	rm -f $(LIBNAME)
	rm -f hello
