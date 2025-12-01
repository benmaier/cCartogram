from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext

ext_modules = [
    Pybind11Extension(
        "cCartogram",
        sources=[
            "cCartogram/cart.cpp",
            "cCartogram/cartogram.cpp",
            "cCartogram/remap_coordinates.cpp",
            "cCartogram/cCartogram.cpp",
        ],
        include_dirs=["./cCartogram/"],
        libraries=["fftw3", "m"],
        extra_compile_args=["-O3"],
        cxx_std=17,
    ),
]

setup(
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
)
