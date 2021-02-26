CC = gcc
AR = ar
SRC = src/cafe.c
INCLUDE = -Imodules/tea/src
MODULES = tea mocha
OUT = cafe
CFLAGS = -std=c99 -Wall -lcafe $(patsubst %,-l%,$(MODULES)) -lSDL2 -lm

LIBNAME = lib$(OUT)



OBJS = $(SRC:%.c=%.o)

$(OUT): setup main.c $(LIBNAME).a
	$(CC) main.c -o bin/$(OUT) -Llib $(CFLAGS) $(INCLUDE)

setup:
	mkdir -p bin
	mkdir -p lib

$(LIBNAME).a: $(OBJS) $(MODULES)
	$(AR) rcs lib/$(LIBNAME).a src/*.o

%.o: %.c
	$(CC) -c $< -o $@ $(INCLUDE) $(CFLAGS)

run: $(OUT)
	./$(OUT)
	
$(MODULES):
	make lib$@.a -C modules/$@
	mv modules/$@/lib$@.a lib/

clean_modules:
	make clean -C modules/tea

clean:
	rm -f $(OUT)
	rm -f $(LIBNAME).a
	rm -f $(OBJS)
	rm -rf bin
	rm -rf lib

clean_all: clean clean_modules
