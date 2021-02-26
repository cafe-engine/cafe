CC = gcc
AR = ar
SRC = src/tea.c
INCLUDE =
OUT = tea
CFLAGS = -std=c99 -Wall -lSDL2 -lm

LIBNAME = libtea

OBJS = $(SRC:%.c=%.o)

$(OUT): main.c $(LIBNAME).a
	$(CC) main.c -o $(OUT) -L. -ltea $(INCLUDE)  $(CFLAGS) $(INCLUDE)

$(LIBNAME).a: $(OBJS)
	$(AR) rcs $(LIBNAME).a src/*.o

%.o: %.c
	$(CC) -c $< -o $@ $(INCLUDE) $(CFLAGS)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)
	rm -f $(LIBNAME).a
	rm -f $(OBJS)
