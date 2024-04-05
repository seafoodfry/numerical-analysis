IMG := lab


.PHONY: all
all: build
	docker run -it -v $(CURDIR):/home/user $(IMG)


.PHONY: build
build:
	docker build -t lab .


.PHONY: clean
clean:
	rm -rf .cache .config .ipynb_checkpoints .ipython .jupyter .local .npm
