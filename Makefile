CC = gcc
AR = ar
SRC = src/cafe.c
INCLUDE = -Iexternal/tea/src
MODULES = tea
OUT = cafe
CFLAGS = -std=c99 -Wall -lSDL2 -lm -ltea

LIBNAME = libcafe

OBJS = $(SRC:%.c=%.o)

$(OUT): main.c $(LIBNAME).a tea
	$(CC) main.c -o $(OUT) -L. -ltea $(CFLAGS) $(INCLUDE)

$(LIBNAME).a: $(OBJS)
	$(AR) rcs $(LIBNAME).a src/*.o

%.o: %.c
	$(CC) -c $< -o $@ $(INCLUDE) $(CFLAGS)

run: $(OUT)
	./$(OUT)
	
tea:
	make libtea.a -C external/tea
	mv external/tea/*.a .

clean:
	rm -f $(OUT)
	rm -f $(LIBNAME).a
	rm -f $(OBJS)
