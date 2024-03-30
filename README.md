# numerical-analysis

## Python Virtual Environment

```
python3 -m venv ./venv
```

## Tests

```
python -m pytest -v
```

## Run Individual Scripts

```
python -m numa.equation_solvers
```

or
```
.venv/bin/python -m numa.equation_systems
```


---

## Style

The [pylintrc](./pylintrc) file came from [google.github.io/styleguide/pyguide.html](https://google.github.io/styleguide/pyguide.html).
We made the following adaptations:

* Changed `max-line-length` from 80 to 120.
* Changed `indent-string` to 4 spaces.
* Added variable names to `good-name`.
