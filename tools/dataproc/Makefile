.PHONY: all clean compdb exe lib

SHELL   = /bin/sh
VERSION = v0.1.0alpha

B_DEBUG    ?= 1
B_SANITIZE ?= 1
D_BUILD    ?= build

### BUILD HELPERS

S_MAKE_COMPILE_COMMANDS_SH := ./tools/make-compile-commands.sh

### COMPILER CONFIGURATION

CFLAGS += -MMD -std=c99 -Iinclude
CFLAGS += -Wall -Wextra -Wconversion -Wno-sign-conversion

CFLAGS += -I../../include
CFLAGS += -I../../build
CFLAGS += -I../../subprojects/narc/lib/include

ifeq ($(B_DEBUG),1)
CFLAGS += -O0 -g3
else
CFLAGS += -O3 -DNDEBUG=1
endif

ifeq ($(B_SANITIZE),1)
CFLAGS  += -fsanitize=address,undefined -fsanitize-trap
LDFLAGS += -fsanitize=address,undefined -fsanitize-trap
endif

LDFLAGS += -L$(D_BUILD)

### SOURCES AND TARGETS

F_LIB_C := $(wildcard lib/*.c)
T_LIB_O := $(patsubst %.c,$(D_BUILD)/%.o,$(F_LIB_C))
T_LIB_D := $(patsubst %.c,$(D_BUILD)/%.d,$(F_LIB_C))

F_SRC_C := $(wildcard src/*.c)
T_SRC_O := $(patsubst %.c,$(D_BUILD)/%.o,$(F_SRC_C))
T_SRC_D := $(patsubst %.c,$(D_BUILD)/%.d,$(F_SRC_C))

T_COMPDB := compile_commands.json

T_DATAPROC_A      := $(D_BUILD)/libdataproc.a
T_SPECIESPROC_EXE := $(D_BUILD)/speciesproc

### RECIPES

all: lib exe

exe: $(T_SPECIESPROC_EXE)

lib: $(T_DATAPROC_A)

clean:
	$(RM) -r $(D_BUILD) $(T_COMPDB)

compdb: $(S_MAKE_COMPILE_COMMANDS_SH)
	B_SANITIZE=0 B_DEBUG=1 exec $< > $(T_COMPDB)

$(T_DATAPROC_A): $(T_LIB_O)
	$(AR) rcs $@ $^

$(T_SPECIESPROC_EXE): $(T_DATAPROC_A) $(T_SRC_O)
	$(CC) $(LDFLAGS) -o $@ $(T_SRC_O) -ldataproc -lnarc

$(D_BUILD)/%.o: %.c | $(D_BUILD)
	$(CC) $(CFLAGS) -c -o $@ $<

$(D_BUILD):
	mkdir -p $(D_BUILD)
	mkdir -p $(D_BUILD)/src $(D_BUILD)/lib
	echo "*" > $(D_BUILD)/.gitignore

-include $(T_SRC_D)
