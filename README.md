# numerical-analysis

## Python Virtual Environment

```
VENV_PATH=~/.poetry
python3 -m venv $VENV_PATH
$VENV_PATH/bin/pip install poetry
```

To add dev dependencies run
```
~/.poetry/bin/poetry add --group=dev scipy
```

To update a package, run

```
~/.poetry/bin/poetry update scipy
```

## Tests

```
make tests
```

## Run Individual Scripts

```
~/.poetry/bin/potery run python -m numa.equation_solvers
```

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