POETRY := ~/.poetry/bin/poetry
IMG := numa-lab

DOCKER_RUN := docker run -it \
	--rm \
	--cap-drop all \
	--security-opt=no-new-privileges \
	--network $(IMG) \
	--memory="2000m" \
	--cpus="2" \
	--ulimit nofile=100 \
	--ulimit nproc=30 \
	-p 8888:8888 \
	-v $(CURDIR):/home/jovyan/work \
	--name $(IMG) \
	$(IMG)

.PHONY: setup
setup:
	${POETRY} install

.PHONY: test
test: setup lint
	${POETRY} run pytest -v

.PHONY: lint
lint:
	${POETRY} run ruff format numa
	${POETRY} run ruff check numa

.PHONY: shell
shell: network build
	${DOCKER_RUN} bash

.PHONY: debug
debug: network build
	${DOCKER_RUN}

.PHONY: exec
exec:
	docker exec -it $(IMG) bash


.PHONY: build
build:
	docker build -t $(IMG) .


.PHONY: network
network:
	docker network inspect $(IMG) || docker network create $(IMG)
	docker network list


.PHONY: clean
clean:
	find . \( -name '*.pyc' -o -name '__pycache__' -o -name '.npm' \) -not -path './.venv/*' -print
	find . \( -name '*.pyc' -o -name '__pycache__' -o -name '.npm' \) -not -path './.venv/*' -print | xargs rm -rf
	docker network rm $(IMG)
