CC = gcc
SRC = src/*.c
OUT = coffee
INCLUDE = -Iexternal -Isrc
CFLAGS = -std=c99 -Wall -lm -lpthread -ldl

LIBNAME = libcoffee

OBJS = $(SRC:%.c=%.o)

$(OUT): main.c $(LIBNAME).a
	$(CC) main.c -o $(OUT) -L. -l$(OUT) $(CFLAGS) $(INCLUDE)

$(LIBNAME).a: $(OBJS)
	ar rcs $(LIBNAME).a src/*.o

%.o: %.c
	$(CC) -c $< -o $@ $(INCLUDE) $(CFLAGS)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)
	rm -f $(LIBNAME).a
	rm -f $(OBJS)
