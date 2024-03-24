IMG := scipy-lab

# OWASP Docker: https://cheatsheetseries.owasp.org/cheatsheets/Docker_Security_Cheat_Sheet.html
# List of capabilities: https://man7.org/linux/man-pages/man7/capabilities.7.html
.PHONY: all
all: network build
	docker run -it \
		--cap-drop all \
		--security-opt=no-new-privileges \
		--network $(IMG) \
		--memory="2000m" \
		--cpus="2" \
		--ulimit nofile=100 \
		--ulimit nproc=30 \
		-v $(CURDIR):/home/user \
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
	rm -rf .cache .local dist mypackage.egg-info
	find . | grep -E "(/__pycache__$$|\.pyc$$|\.pyo$$)"
	find . | grep -E "(/__pycache__$$|\.pyc$$|\.pyo$$)" | xargs rm -rf
	rm -rf .bash_history .viminfo
	docker network rm $(IMG)

