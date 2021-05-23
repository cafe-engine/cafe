NAME = cafe
CFLAGS = -Wall -std=c99 `sdl2-config --cflags` 
LFLAGS = `sdl2-config --libs`

SRC = cafe.c
MAIN = main.c

TARGET = 
PREFIX ?= 

CDEFS = 

CC ?= cc
AR ?= ar

LIBNAME = lib$(NAME)
SLIBNAME = $(LIBNAME).a
DLIBNAME = $(LIBNAME).so

MENU = tea mocha
MENU_FOLDERS = $(MENU:%=menu/%/)
MENU_FILES = $(join $(MENU_FOLDERS),$(MENU))

INCLUDE = $(MENU_FOLDERS:%=-I%) $(MENU_FOLDERS:%=-I%external)
MENU_OBJ = $(MENU_FILES:%=%.o)

OBJ = cafe.o $(MENU_OBJ)
DOBJ = cafe.d.o $(MENU_OBJ:%.o=%.d.o)

ifeq ($(OS),Windows_NT)
    LFLAGS += -mwindows
else
    LFLAGS += -lpthread -lm -ldl
endif

OUT = $(NAME)

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

clean-all: clean $(CLEAN_MENU)
