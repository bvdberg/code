# Compiler/Linker/dynamic linker
CC = cc
LD = cc

# flags to compile object files that can be used in a dynamic library
CFLAGS = -fPIC
# on some platforms, use '-fpic' instead.

# Flags to create a dynamic library.
DYNLINKFLAGS = -shared -nostdlib
# on some platforms, use '-G' instead.

# files removal
RM = /bin/rm -f

# program's object files
PROG_OBJS = main.o

# program's executable
PROG = prog

# libraries to link with the program during compile time.
LIBS = -ldl

# shared library files
LIB_FILES = lib.so

# shared libraries object files
LIB_OBJS = lib.o

# top-level rule
all: $(LIB_FILES) $(PROG)

$(PROG): $(PROG_OBJS)
	$(LD) $(LDFLAGS) $(PROG_OBJS) $(LIBS) -o $(PROG)

# create our librarys
lib.so: lib.o
	$(LD) $(DYNLINKFLAGS) $< -o $@

# compile C source files into object files.
%.o: %.c
	$(CC) $(CFLAGS) -c $<


# clean everything
clean:
	$(RM) $(PROG_OBJS) $(PROG) $(LIB_OBJS) $(LIB_FILES) test

# clean the program and its object files, don't touch the library.
cleanprog:
	$(RM) $(PROG_OBJS) $(PROG)

# clean the library and its object files only
cleanlib:
	$(RM) $(LIB_OBJS) $(LIB_FILES)

# clean the library's object files only
cleanlibobjs:
	$(RM) $(LIB_OBJS)

test: main.c
	$(CC) main.c -o test

