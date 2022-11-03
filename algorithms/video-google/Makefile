IMG := video-google

all:
	docker build -t $(IMG) .
	docker run -it -v $(CURDIR):/src $(IMG) bash
