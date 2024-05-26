IMG := qft-lab
#CONTAINER_NTWR := --network $(IMG)
CONTAINER_NTWR := -p 8888:8888


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

.PHONY: network
network:
	docker network inspect $(IMG) || docker network create $(IMG)
	docker network list

.PHONY: clean
clean:
	docker network rm $(IMG)
