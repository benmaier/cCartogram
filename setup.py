#This file is forked from https://github.com/pybind/pbtest, original author: Sylvain Corlay

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import setuptools
import os, sys
from pip import locations

class get_pybind_include(object):

    def __init__(self,user=False):
        self.user = user

    def __str__(self):
        pybind_include = os.path.dirname(locations.distutils_scheme('pybind11',self.user)['headers'])
        return pybind_include

ext_modules = [
    Extension(
        'cCartogram',
        [ 
            'cCartogram/cart.cpp', 
            'cCartogram/cartogram.cpp', 
            'cCartogram/remap_coordinates.cpp', 
            'cCartogram/cCartogram.cpp', 
        ],
        include_dirs=[
            get_pybind_include(),
            get_pybind_include(user=True),
            "./cCartogram/",
        ],
        libraries = ['fftw3', 'm'],
        library_dirs = ['/usr/local/lib'],
        language='c++',
        extra_compile_args = ["-O3"], 
    ),
]

def has_flag(compiler, flagname):
    """Return a boolean indicating whether a flag name is supported on
    the specified compiler.
    """
    import tempfile
    fd, fname = tempfile.mkstemp('.cpp', 'main', text=True)
    with os.fdopen(fd, 'w') as f:
        f.write('int main (int argc, char **argv) { return 0; }')
    try:
        compiler.compile([fname], extra_postargs=[flagname])
    except setuptools.distutils.errors.CompileError:
        return False
    return True

def cpp_flag(compiler):
    """Return the -std=c++[11/14] compiler flag.
    The c++14 is preferred over c++11 (when it is available).
    """
    if has_flag(compiler, '-std=c++14'):
        return '-std=c++14'
    elif has_flag(compiler, '-std=c++11'):
        return '-std=c++11'
    else:
        raise RuntimeError('Unsupported compiler -- at least C++11 support is needed!')


class BuildExt(build_ext):
    """A custom build extension for adding compiler-specific options."""
    c_opts = {
        'msvc': ['/EHsc'],
        'unix': [],
    }

    if sys.platform == 'darwin':
        c_opts['unix'] += ['-stdlib=libc++', '-mmacosx-version-min=10.7','-ftemplate-depth=1024']

    def build_extensions(self):
        ct = self.compiler.compiler_type
        opts = self.c_opts.get(ct, [])
        if ct == 'unix':
            opts.append(cpp_flag(self.compiler))
            if has_flag(self.compiler, '-fvisibility=hidden'):
                opts.append('-fvisibility=hidden')
        for ext in self.extensions:
            ext.extra_compile_args = opts
        build_ext.build_extensions(self)

setup(
    name='cCartogram',
    version='0.0.1',
    author='Mark Newman, Benjamin F. Maier',
    author_email='bfmaier@physik.hu-berlin.de',
    url='https://github.com/benmaier/pycartogram',
    license='custom',
    description="A Python wrapper for Mark Newman's C implementation of the density-equalizing cartogram algorithm.",
    #packages=['pycartogram'],
    long_description='',
    ext_modules=ext_modules,
    install_requires=['pybind11'],
    cmdclass={'build_ext': BuildExt},
    zip_safe=False,
)
