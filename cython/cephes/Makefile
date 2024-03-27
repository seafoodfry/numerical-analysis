CC := gcc-13
CFLAGS = -g -O2 -Wall -fno-builtin -Werror
AR := gcc-ar-13

LIB := cephes
LIB_FILENAME := lib$(LIB).a
EXEC := my_project_executable

.PHONY: all
all:
	# Compile the library.
	#$(CC) $(CFLAGS) -c clog.c -o clog.o
	#$(CC) $(CFLAGS) -c cmplx.c -o cmplx.o
	$(CC) $(CFLAGS) -c polevl.c -o polevl.o
	# Create a static or shared library.
	# ar rcs libcephes.a *.o
	# gcc -shared -o libcephes.so *.o
	$(AR) rcs $(LIB_FILENAME) *.o
	# Compile my_project.c into an object file, assuming cephes headers are in the current directory or a standard location
	$(CC) $(CFLAGS) -c project.c -o project.o -I.
	# Link the object file with libcephes.a to produce the final executable, assuming libcephes.a is in the current directory
	$(CC) project.o -o $(EXEC) -L. -l$(LIB)



.PHONY: clean
clean:
	rm -f *.o
	rm -f libcephes.a
	rm -f $(EXEC)
