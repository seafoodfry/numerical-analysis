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

## Run jupyterLab

```
make debug
```