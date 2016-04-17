
CC = cc

CFLAGS := -Wall -Wextra -pedantic -g -Wwrite-strings
GRAPHICS_FLAGS  := -lGLEW -lglfw3 -lGL -lX11 -lXrandr -lXi -lXxf86vm -lm -ldl -lXinerama -lXcursor -lrt -lpthread
SOUND_FLAGS :=  -lportaudio -lasound

# Tracked folders.
D_SRC := source
D_INC := include
D_SUP := valgrind_suppression

# Generated folders.
D_EXEC := executables
D_OBJ := objects

INCLUDE_FLAGS := -I$(D_INC)

# Supression files.
SUP_MEM := $(D_SUP)/memory_leak_suppression.txt

# Command list.
SUP_FLAGS := --suppressions=$(SUP_MEM)
MEM_FLAGS := --leak-check=full --show-leak-kinds=all
DUMP_AND_OPEN := > dump.txt 2>&1 && vim dump.txt

.PHONY: clean
.PHONY: all
.PHONY: val
.PHONY: test

all: falling_pixels test_falling_pixels

val: all
	valgrind $(SUP_FLAGS) $(D_EXEC)/test_falling_pixels

valmem: all
	valgrind $(SUP_FLAGS) $(MEM_FLAGS) $(D_EXEC)/test_falling_pixels

valdump: all
	valgrind $(SUP_FLAGS) $(MEM_FLAGS) $(D_EXEC)/test_falling_pixels $(DUMP_AND_OPEN)

test: all
	$(D_EXEC)/test_falling_pixels

clean:
	$(info Removing folder: $(D_OBJ).)
	@rm -rf $(D_OBJ)
	$(info Removing folder: $(D_EXEC).)
	@rm -rf $(D_EXEC)
	$(info Removing vgcore files.)
	@rm -rf vgcore*
	$(info Done.)

falling_pixels: $(D_OBJ)/falling_pixels.o | mk_$(D_EXEC)
	$(CC) -o $(D_EXEC)/falling_pixels $(D_OBJ)/falling_pixels.o $(CFLAGS) $(GRAPHICS_FLAGS)

$(D_OBJ)/falling_pixels.o: $(D_SRC)/falling_pixels.c $(D_INC)/falling_pixels.h $(D_INC)/utility_functions.h | mk_$(D_OBJ)
	$(CC) -c $(D_SRC)/falling_pixels.c -o $(D_OBJ)/falling_pixels.o \
	$(GRAPHICS_FLAGS) $(CFLAGS) $(INCLUDE_FLAGS)

# Utility modules.

$(D_OBJ)/utility_functions.o: 	$(D_SRC)/utility_functions.c \
				$(D_SRC)/utils/utils_dict.c \
				$(D_SRC)/utils/utils_config_parser.c
	$(CC) -c $(D_SRC)/utility_functions.c -o $(D_OBJ)/utility_functions.o $(CFLAGS) $(INCLUDE_FLAGS)

# Test modules.

test_falling_pixels: $(D_OBJ)/test_falling_pixels.o $(D_OBJ)/utility_functions.o
	$(CC) -o $(D_EXEC)/test_falling_pixels $(D_OBJ)/test_falling_pixels.o \
	       	$(D_OBJ)/utility_functions.o $(CFLAGS) $(GRAPHICS_FLAGS)

$(D_OBJ)/test_falling_pixels.o: $(D_SRC)/test_falling_pixels.c $(D_INC)/falling_pixels.h $(D_INC)/minunit.h $(D_INC)/utility_functions.h
	$(CC) -c $(D_SRC)/test_falling_pixels.c -o $(D_OBJ)/test_falling_pixels.o \
		$(GRAPHICS_FLAGS) $(CFLAGS) $(INCLUDE_FLAGS)

# Utility commands.

mk_$(D_OBJ):
ifeq "$(wildcard $(D_OBJ))" ""
-include $(shell mkdir $(D_OBJ)) $(wildcard $(D_OBJ)/*)
endif

mk_$(D_EXEC):
ifeq "$(wildcard $(D_EXEC))" ""
-include $(shell mkdir $(D_EXEC)) $(wildcard $(D_EXEC)/*)
endif



#dummy: dummy_printer.o print_function.o
#	$(CC) $(CFLAGS) -o dummy_printer dummy_printer.o print_function.o

#dummy_printer.o: $(D_SRC)/dummy_printer.c
#	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -c $(D_SRC)/dummy_printer.c

#print_function.o: $(D_SRC)/print_function.c $(D_INC)/print_function.h $(D_INC)/text.h
#	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -c $(D_SRC)/print_function.c

#falling_pixels: falling_pixels.o utility_functions.o
#	$(CC) $(CFLAGS) $(GRAPHICS_FLAGS) -o falling_pixels falling_pixels.o utility_functions.o


#falling_pixels: $(D_SRC)/falling_pixels.c
#	$(CC) $(D_SRC)/falling_pixels.c $(CFLAGS) $(GRAPHICS_FLAGS) $(INCLUDE_FLAGS) -o $(D_EXEC)/falling_pixels

#test_falling_pixels: test_falling_pixels.o utility_functions.o
#	$(CC) $(CFLAGS) $(GRAPHICS_FLAGS) -o test_falling_pixels test_falling_pixels.o utility_functions.o


#test_falling_pixels.o: $(D_SRC)/test_falling_pixels.c $(D_INC)/falling_pixels.h $(D_INC)/minunit.h
#	$(CC) $(CFLAGS) $(GRAPHICS_FLAGS) $(INCLUDE_FLAGS) -c $(D_SRC)/test_falling_pixels.c

#utility_functions.o: $(D_SRC)/utility_functions.c
#	$(CC) $(CFLAGS) $(GRAPHICS_FLAGS) -c $(D_SRC)/utility_functions.c

