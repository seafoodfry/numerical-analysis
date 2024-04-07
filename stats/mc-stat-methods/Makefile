IMG := lab #quay.io/jupyter/scipy-notebook:python-3.11


.PHONY: all
all: build
	docker run -it -p 8888:8888 -v $(CURDIR):/home/jovyan/work $(IMG)


.PHONY: build
build:
	docker build -t lab .


.PHONY: clean
clean:
	rm -rf .cache .config .ipynb_checkpoints .ipython .jupyter .local .npm
