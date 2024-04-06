IMG := lab


.PHONY: all
all: build
	docker run -it -p 8888:8888 -v $(CURDIR):/home/jovyan/work $(IMG)


.PHONY: build
build:
	docker build -t lab .


.PHONY: run2dwalk
run2dwalk:
	g++ -Wall -I/usr/local/include -c Basic2D.cpp
	g++ -o Basic2D Basic2D.o -L/usr/local/lib -Wl,-rpath,/usr/local/lib -lgsl -lgslcblas -lm
	@bash -c "time ./Basic2D"

.PHONY: clean
clean:
	rm -rf *.o *.so *.csv build/
	rm -rf Basic2D
