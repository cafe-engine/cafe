NAME = cafe
CC = cc
AR = ar
CFLAGS = -Wall -std=c99
LFLAGS =
OUT = $(NAME)
CLEAR_FILES = 

LUA_SRC = $(wildcard external/lua/src/*.c)

SRC = cafe.c lua/cafe.c $(LUA_SRC)
MAIN = main.c
INCLUDE =

ifeq ($(TARGET),Web)
    CLEAN_FILES = $(NAME).wasm $(NAME).js
endif

ifeq ($(OS),Windows_NT)
    TARGET ?= Windows
    LIB_EXT = dll
else
    UNAME_S = $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
    TARGET ?= OSX
    LIB_EXT = dylib
    else
    TARGET ?= $(UNAME_S)
    endif
endif

include cross/Makefile.$(TARGET)

LIBNAME = lib$(NAME)
SLIBNAME = $(LIBNAME).a
DLIBNAME = $(LIBNAME).$(LIB_EXT)

MENU = tea mocha coffee latte
MENU_FOLDERS = $(MENU:%=menu/%/)
MENU_FILES = $(join $(MENU_FOLDERS),$(MENU))

INCLUDE += -Iexternal/lua/src/ $(MENU_FOLDERS:%=-I%) $(MENU_FOLDERS:%=-I%external)
MENU_OBJ = $(MENU_FILES:%=%.o)

OBJ = $(SRC:%.c=%.o) $(MENU_OBJ)
DOBJ = cafe.do $(MENU_OBJ:%.o=%.do)

CLEAN_MENU = $(MENU:%=%.cls)

.PHONY: all build $(MENU)
.SECONDARY: $(OBJ) $(DOBJ)

build: $(NAME)

all: $(SLIBNAME) $(DLIBNAME) $(OUT)

$(NAME): $(SLIBNAME) $(MAIN)
	@echo "********************************************************"
	@echo "** COMPILING $@"
	@echo "********************************************************"
	$(CC) $(MAIN) -o $(OUT) $(INCLUDE) $(CFLAGS) -L. -l$(NAME) $(LFLAGS) $(CDEFS)
	@echo ""

%.a: $(OBJ)
	@echo "********************************************************"
	@echo "** CREATING $@"
	@echo "********************************************************"
	$(AR) rcs $@ $(OBJ)
	@echo ""

%.so: $(DOBJ)
	@echo "********************************************************"
	@echo "** CREATING $@"
	@echo "********************************************************"
	$(CC) -shared -o $@ $(DOBJ) $(INCLUDE) $(CFLAGS) $(CDEFS)
	@echo ""
%.o: %.c
	@echo "********************************************************"
	@echo "** $(SLIBNAME): COMPILING SOURCE $<"
	@echo "********************************************************"
	@mkdir -p '$(@D)'
	$(CC) -c $< -o $@ $(INCLUDE) $(CFLAGS) $(CDEFS)

%.do: %.c
	@echo "********************************************************"
	@echo "** $(DLIBNAME): COMPILING SOURCE $<"
	@echo "********************************************************"
	@mkdir -p '$(@D)'
	$(CC) -c $< -o $@ -fPIC $(INCLUDE) $(CFLAGS) $(CDEFS)

$(MENU):
	$(MAKE) -C menu/$@

%.cls: menu/%
	echo $<
	$(MAKE) clean -C $<

clean:
	rm -rf $(OUT) $(CLEAR_FILES) 
	rm -rf $(OBJ) $(DOBJ)
	rm -rf $(DLIBNAME) $(SLIBNAME)
	rm -rf $(FOLDERS)

clean-all: clean $(CLEAN_MENU)
