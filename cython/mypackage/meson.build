project(
  'mypackage',
  'cython',
  # Note that the git commit hash cannot be added dynamically here (it is added
  # in the dynamically generated and installed `scipy/version.py` though - see
  # tools/version_utils.py
  version: '1.14.0.dev0',
  meson_version: '>= 1.1.0',
)

py3 = import('python').find_installation(pure: false)
py3_dep = py3.dependency()

#py3.extension_module(
#    'mypackage',
#    dependencies : py3_dep,
#)

cython_sources = ['mypackage/hello.pyx']  # Specify your Cython sources here

py3.extension_module(
    'hello',  # This should match the module name in your Cython file
    sources : cython_sources,
    dependencies : py3_dep,
    install : true,
    subdir: 'mypackage'  # Adjust if necessary
)
