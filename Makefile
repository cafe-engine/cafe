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

ifeq ($(PLATFORM), Windows)
include cross/mingw.makefile
else
    ifeq ($(PLATFORM),Web)
    include cross/emscripten.makefile
    else
    include cross/linux.makefile
    endif
endif

TARGET = 
PREFIX ?= 

CDEFS = 

LIBNAME = lib$(NAME)
SLIBNAME = $(LIBNAME).a
DLIBNAME = $(LIBNAME).so

MENU = tea mocha coffee
MENU_FOLDERS = $(MENU:%=menu/%/)
MENU_FILES = $(join $(MENU_FOLDERS),$(MENU))

INCLUDE += -Iexternal/lua/src/ $(MENU_FOLDERS:%=-I%) $(MENU_FOLDERS:%=-I%external)
MENU_OBJ = $(MENU_FILES:%=%.o)

OBJ = $(SRC:%.c=%.o) $(MENU_OBJ)
DOBJ = cafe.d.o $(MENU_OBJ:%.o=%.d.o)

CLEAN_MENU = $(MENU:%=%.cls)

.PHONY: all build $(MENU)
.SECONDARY: $(OBJ) $(DOBJ)

build: $(OUT)

all: $(SLIBNAME) $(DLIBNAME) $(OUT)

$(OUT): $(SLIBNAME) $(MAIN)
	@echo "********************************************************"
	@echo "** COMPILING $@"
	@echo "********************************************************"
	$(CC) $(MAIN) -o $@ $(INCLUDE) $(CFLAGS) -L. -l$(NAME) $(LFLAGS) $(CDEFS)
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

%.d.o: %.c
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
	rm -rf $(OUT)
	rm -rf $(OBJ)
	rm -rf $(DLIBNAME) $(SLIBNAME)
	rm -rf $(FOLDERS)
	rm -rf $(CLEAR_FILES)

clean-all: clean $(CLEAN_MENU)
