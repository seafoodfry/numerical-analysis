IMG := lab #quay.io/jupyter/scipy-notebook:python-3.11

all: build
	docker run -it -p 8888:8888 -v $(CURDIR):/home/jovyan $(IMG)

build:
	docker build -t lab .
