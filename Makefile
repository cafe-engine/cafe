NAME ?= hello
CSTD = c99
CFLAGS =
LFLAGS = 
MAIN = main.c

TARGET = 
PREFIX ?= 

MODULES = 
CDEFS ?= 

CC ?= cc
AR ?= ar

LIBNAME = lib$(NAME)
SLIBNAME = $(LIBNAME).a
DLIBNAME = $(LIBNAME).so

BIN_DIR ?= bin
OBJ_DIR ?= obj
LIB_DIR ?= lib

MODDIR = modules

SRC_DIR = src
INC_DIR = include

INCLUDE =

-include config.mak

SRC += $(wildcard $(SRC_DIR)/*.c)
INCLUDE += -I$(INC_DIR) -I$(SRC_DIR)
INCLUDE += $(MODULES:%=-I$(MODDIR)/%/src) $(MODULES:%=-I$(MODDIR)/%/include)

OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)
SOBJ = $(OBJ:%.o=%.s.o)
DOBJ = $(OBJ:%.o=%.d.o)

FOLDERS = $(OBJ_DIR) $(LIB_DIR) $(BIN_DIR)

CFLAGS += -Wall -std=$(CSTD)

MODS = $(MODULES:%=$(MODDIR)/%)

CROSS_CC = $(PREFIX)$(CC)
CROSS_AR = $(PREFIX)$(AR)

SLIBOUT = $(SLIBNAME:%=$(LIB_DIR)/$(SLIBNAME))
DLIBOUT = $(DLIBNAME:%=$(LIB_DIR)/$(DLIBNAME))
OUT = $(NAME:%=$(BIN_DIR)/%)

CLEAN_MODULES = $(MODULES:%=%.cls)

build: folders $(MODULES) $(OUT)

folders: $(FOLDERS)

all: folders $(SLIBOUT) $(DLIBOUT) $(OUT)

.PHONY: all build folders $(MODULES)
.SECONDARY: $(SOBJ) $(DOBJ)


$(FOLDERS):
	@mkdir -p $@

$(OUT): $(MAIN) $(SLIBOUT) 
	@echo "********************************************************"
	@echo "** COMPILING $@"
	@echo "********************************************************"
	$(CROSS_CC) $(MAIN) -o $@ $(INCLUDE) $(CFLAGS) -L$(LIB_DIR) -l$(NAME) $(LFLAGS) $(CDEFS)
	@echo ""

%.a: $(SOBJ)
	@echo "********************************************************"
	@echo "** CREATING $@"
	@echo "********************************************************"
	$(CROSS_AR) rcs $@ $(shell find $(OBJ_DIR)/ -name "*.o")
	@echo ""

%.so: $(DOBJ)
	@echo "********************************************************"
	@echo "** CREATING $@"
	@echo "********************************************************"
	$(CROSS_CC) -shared -o $@ $(shell find $(OBJ_DIR)/ -name "*.d.o") $(INCLUDE) $(CFLAGS) $(CDEFS)
	@echo ""

$(OBJ_DIR)/%.s.o: %.c
	@echo "********************************************************"
	@echo "** $(SLIBNAME): COMPILING SOURCE $<"
	@echo "********************************************************"
	@mkdir -p '$(@D)'
	$(CROSS_CC) -c $< -o $@ $(INCLUDE) $(CFLAGS) $(CDEFS)

$(OBJ_DIR)/%.d.o: %.c
	@echo "********************************************************"
	@echo "** $(DLIBNAME): COMPILING SOURCE $<"
	@echo "********************************************************"
	@mkdir -p '$(@D)'
	$(CROSS_CC) -c $< -o $@ -fPIC $(INCLUDE) $(CFLAGS) $(CDEFS)

$(MODULES):
	$(MAKE) -C $(MODDIR)/$@

%.cls: $(MODDIR)/%
	echo $<
	$(MAKE) clean -C $<

clean:
	rm -rf $(OUT)
	rm -rf $(DLIBNAME) $(SLIBNAME)
	rm -rf $(FOLDERS)

clean-all: clean $(CLEAN_MODULES)
