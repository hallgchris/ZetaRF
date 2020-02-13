from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize

zetarf433_extension = Extension(
    name="zetarf433",
    sources=["zetarf433.pyx"],
    libraries=["zetarf", "wiringPi"],
    library_dirs=["build"],
    include_dirs=["src"],
)

setup(
    name="zetarf",
    ext_modules=cythonize([zetarf433_extension]),
)
