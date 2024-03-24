# Mypackage

```
meson setup builddir
```

```
meson compile -C builddir/
```

One could also run a `meson install` command but we'll skip it since we'll be using `pip install -e`.

After this, we need to copy the compiled artifact, because `pip install -e` will
not link data from the MANIFEST.in file.

```
cp builddir/hello.cpython-312-aarch64-linux-gnu.so mypackage/
```
(A symbolic link doesn't work.)

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
