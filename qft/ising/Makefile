IMG := ising-lab
#CONTAINER_NTWR := --network $(IMG)
CONTAINER_NTWR := -p 8888:8888

CC := g++
# Compiler flags:
# -g    adds debugging information to the executable file
# -Wall turns on most compiler warnings
# -Wextra https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html#index-Wextra
# -Werror Make all warnings into errors. 
CFLAGS := -g -O2 -Wall -Wextra -Werror 
LFLAGS := -L/usr/local/lib -Wl,-rpath,/usr/local/lib -lgsl -lgslcblas -lm

TARGET = Metropolis
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)


# OWASP Docker: https://cheatsheetseries.owasp.org/cheatsheets/Docker_Security_Cheat_Sheet.html
# List of capabilities: https://man7.org/linux/man-pages/man7/capabilities.7.html
# NOTE: we did not `--cap-drop all` as we needed some for jupyter.
.PHONY: all
all: network build
	docker run -it \
		--security-opt=no-new-privileges \
		--memory="2000m" \
		--cpus="3" \
		--ulimit nofile=100 \
		--ulimit nproc=60 \
		$(CONTAINER_NTWR) \
		-v $(CURDIR):/home/jovyan/work \
		$(IMG) bash

.PHONY: build
build:
	docker build -t $(IMG) .

.PHONY: compile
compile:
	$(CC) $(CFLAGS) -c HashTable.cpp

# Rule to link the program.
$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LFLAGS)

# Rule to compile every .cpp to an .o
# The -c flag says to generate the object file,
# the -o $@ says to put the output of the compilation in the file named on the left side of the :,
# the $< is the first item in the dependencies list, and CXXFLAGS are the flags passed to the compiler.
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


.PHONY: network
network:
	docker network inspect $(IMG) || docker network create $(IMG)
	docker network list

.PHONY: clean
clean: clean-data
	docker network rm $(IMG)


.PHONY: clean-data
clean-data:
	rm -rf *.o *.txt *.csv
