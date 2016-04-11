
CC = cc

CFLAGS = -Wall -Wextra -pedantic -g
GRAPHICS_FLAGS  = -lGLEW -lglfw3 -lGL -lX11 -lXrandr -lXi -lXxf86vm -lm -ldl -lXinerama -lXcursor -lrt -lpthread
SOUND_FLAGS =  -lportaudio -lasound

D_SRC = source
D_INC = include
D_EXEX = executables

INCLUDE_FLAGS = -I$(D_INC)

#CFLAGS = -lGLEW -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -lXxf86vm -lm -ldl -lXinerama -lXcursor -lrt -lasound -lpthread -lportaudio -Wall -Wextra -pedantic
#GRAPHICS_FLAGS =



falling_pixels: falling_pixels.o

# FAILS #$(CC) $(CFLAGS) $(GRAPHICS_FLAGS) -o $(D_EXEX)/falling_pixels falling_pixels.o
#
# Works #$(CC) -o $(D_EXEX)/falling_pixels falling_pixels.o $(CFLAGS) $(GRAPHICS_FLAGS)
# Works #$(CC) $(CFLAGS) -o $(D_EXEX)/falling_pixels falling_pixels.o $(GRAPHICS_FLAGS)

# Works O.o #$(CC) $(CFLAGS) $(GRAPHICS_FLAGS) -o $(D_EXEX)/falling_pixels falling_pixels.o $(GRAPHICS_FLAGS)
# FAILS #$(CC) $(CFLAGS) $(GRAPHICS_FLAGS) -o $(D_EXEX)/falling_pixels falling_pixels.o
# FAILS #$(CC) $(GRAPHICS_FLAGS) -o $(D_EXEX)/falling_pixels falling_pixels.o $(CFLAGS)
	$(CC) -o $(D_EXEX)/falling_pixels falling_pixels.o $(CFLAGS) $(GRAPHICS_FLAGS)

falling_pixels.o: $(D_SRC)/falling_pixels.c $(D_INC)/falling_pixels.h
	$(CC) -c $(D_SRC)/falling_pixels.c $(GRAPHICS_FLAGS) $(CFLAGS) $(INCLUDE_FLAGS)




#dummy: dummy_printer.o print_function.o
#	$(CC) $(CFLAGS) -o dummy_printer dummy_printer.o print_function.o

#dummy_printer.o: $(D_SRC)/dummy_printer.c
#	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -c $(D_SRC)/dummy_printer.c

#print_function.o: $(D_SRC)/print_function.c $(D_INC)/print_function.h $(D_INC)/text.h
#	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -c $(D_SRC)/print_function.c

#falling_pixels: falling_pixels.o utility_functions.o
#	$(CC) $(CFLAGS) $(GRAPHICS_FLAGS) -o falling_pixels falling_pixels.o utility_functions.o


#falling_pixels: $(D_SRC)/falling_pixels.c
#	$(CC) $(D_SRC)/falling_pixels.c $(CFLAGS) $(GRAPHICS_FLAGS) $(INCLUDE_FLAGS) -o $(D_EXEX)/falling_pixels

#test_falling_pixels: test_falling_pixels.o utility_functions.o
#	$(CC) $(CFLAGS) $(GRAPHICS_FLAGS) -o test_falling_pixels test_falling_pixels.o utility_functions.o


#test_falling_pixels.o: $(D_SRC)/test_falling_pixels.c $(D_INC)/falling_pixels.h $(D_INC)/minunit.h
#	$(CC) $(CFLAGS) $(GRAPHICS_FLAGS) $(INCLUDE_FLAGS) -c $(D_SRC)/test_falling_pixels.c

#utility_functions.o: $(D_SRC)/utility_functions.c
#	$(CC) $(CFLAGS) $(GRAPHICS_FLAGS) -c $(D_SRC)/utility_functions.c

