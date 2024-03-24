# Mypackage

```
python -m build
```

> setuptools allows you to install a package without copying any files to your interpreter directory
> (e.g. the site-packages directory). This allows you to modify your source code and have the changes
>  take effect without you having to rebuild and reinstall. Here’s how to do it:
xref: [setuptools: quickstart](https://setuptools.pypa.io/en/latest/userguide/quickstart.html)
```
pip install --editable .
```

This is the reason we added `~/.local/bin` to the `PATH`.

---

```
meson setup builddir
```

```
meson compile -C builddir/
```
