# numerical-analysis

## Python Virtual Environment

Install uv!

---
## Dep management

For dev deps:
```
uv add --dev pytest
```

---
## Tests

```
make tests
```

---
## Run Individual Scripts

```
~/.poetry/bin/potery run python -m numa.equation_solvers
```

---
## Run Jupyter Lab

```
make debug
```


---

# Animations and Images

### CFD

1. [Heatmap-like animations](./CFDPython/Animations-101.ipynb) (animation) heatmap-like animations.

## QFT

1. [Ising model figures](./qft/ising/Ising-model.ipynb) (animation)
1. [Branch cut vizualizations](./qft/peskin-schroeder/Integrals.ipynb) (animation)

## Stats

Mostly single and 1x2 plots.

1. [Normal distribution](./stats/basics/01-continuous-distributions.ipynb) - lines, bar graphs, fill in between, with text.
1. [Confidence intervals](./stats/basics/02-confidence-intervals.ipynb) - quantiles.
1. [Hypothesis tests](./stats/basics/03-hypothesis-testing.ipynb) - quantiles, lines, power of test plots.

1. [Generating random variables](./stats/mc-stat-methods/01-generating-random-variables.ipynb) - 3 images on a 2x2, triangle with equations, multiple y-axis


---

# MISC Docker

## Network

To provide another layer of isolation, we can run containers in a separate network:
```
IMG := qft-lab
CONTAINER_NTWR := --network $(IMG)

docker run -it \
    ...
    $(CONTAINER_NTWR) \
    ...
	$(IMG)
```

```Makefile
.PHONY: network
network:
	docker network inspect $(IMG) || docker network create $(IMG)
	docker network list

.PHONY: clean
clean:
	docker network rm $(IMG)
```


## Non-root User

```Dockerfile
FROM quay.io/jupyter/scipy-notebook:python-3.11

WORKDIR /tmp

USER root

RUN apt-get update -y && \
    apt-get install vim less wget gnupg build-essential -y


WORKDIR /home/jovyan/work

RUN apt-get update -y && \
    apt-get install bash-completion -y #&& \
#    python3 -m venv /opt/venv
#ENV PATH=/opt/venv/bin:$PATH

# Manually creating a user. The other option is to run with
# -u $(id -u ${USER}):$(id -g ${USER})
# Another good example: https://github.com/jupyter/docker-stacks/blob/b2efa3cd8ce7a2b900f76ff4a17e11a000666cea/images/docker-stacks-foundation/Dockerfile#L72
#RUN useradd --no-log-init --create-home --shell /bin/bash --uid 1000 --no-user-group  user && \
#    chown -R user /home/user && \
#    chown -R user /opt/venv

#USER user
USER $NB_UID

#ENV PATH=/opt/venv/bin:$PATH
ENV PATH=$PATH:/home/user/.local/bin

COPY deps/requirements.txt .
RUN . /etc/profile.d/bash_completion.sh && \
    pip install -Ur requirements.txt
```
