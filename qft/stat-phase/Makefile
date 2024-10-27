.PHONY: debug
debug:
	docker build -t debug -f Dockerfile.dev .
	docker run -it \
		-v $(PWD)/stationary-phase:/src/app \
		debug bash

.PHONY: run-app
run-app:
	docker build -t app -f Dockerfile .
	docker run -p 8080:80 app