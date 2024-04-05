# Random Walks

GSL key obtained from [www.gnu.org/software/gsl/key/gsl_key.txt](https://www.gnu.org/software/gsl/key/gsl_key.txt)
on April 5, 2024.

```
Libraries have been installed in:
   /usr/local/lib

If you ever happen to want to link against installed libraries
in a given directory, LIBDIR, you must either use libtool, and
specify the full pathname of the library, or use the '-LLIBDIR'
flag during linking and do at least one of the following:
   - add LIBDIR to the 'LD_LIBRARY_PATH' environment variable
     during execution
   - add LIBDIR to the 'LD_RUN_PATH' environment variable
     during linking
   - use the '-Wl,-rpath -Wl,LIBDIR' linker flag
   - have your system administrator add LIBDIR to '/etc/ld.so.conf'

See any operating system documentation about shared libraries for
more information, such as the ld(1) and ld.so(8) manual pages.
```

We decided against using ATLAS because it is not maintained.
Its latest stable release was on 2016.
The developer one in 2018.
And the errate file for 3.10.2 said

> Performance is terrible for AVX or later SIMD vectorization or large scale parallel architectures.
> These platforms should probably use the developer release, which can more than double your performance. 

But before we discovered that, we did download it and got this hash for it:

```
sha256sum atlas3.10.3.tar.bz2
2688eb733a6c5f78a18ef32144039adcd62fabce66f2eb51dd59dde806a6d2b7  atlas3.10.3.tar.bz2
```


Compiled as:
```
g++ -Wall -I/usr/local/include -c Basic2D.cpp
```

And linked as:
```
g++ -o Basic2D Basic2D.o -L/usr/local/lib -Wl,-rpath,/usr/local/lib -lgsl -lgslcblas -lm
```

Note that the order of arguments when linking does matter.
Also, we are using g++ because gcc doesn't by default link against the C++ standard library.
Note that we also added the `-Wl` flag to pass the `-rpath` flag directly to the linker, so that it knew where
to find `libgsl.so` without having to use the `-static` flag when compiling or having to do `export LD_LIBRARY_PATH=/usr/local/lib`.
