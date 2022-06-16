# -*- coding: utf-8 -*-
import os
import sys
import subprocess

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=""):
        super().__init__(name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        # extdir = os.path.join(extdir, self.distribution.get_name())
        print("extdir:", extdir)

        build_type = "Debug" if self.debug else "Release"

        # Set Python_EXECUTABLE instead if you use PYBIND11_FINDPYTHON
        # DCMAKE_BUILD_TYPE is not used on MSVC
        cmake_args = [
            "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={}".format(extdir),
            "-DPYTHON_EXECUTABLE={}".format(sys.executable),
            "-DCMAKE_BUILD_TYPE={}".format(build_type),
        ]
        build_args = []

        if self.parallel is None:
            self.parallel = 4
        build_args.append("-j{}".format(self.parallel))

        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)

        print("cmake_args:", cmake_args)
        print("build_args:", build_args)
        print("build_temp:", self.build_temp)

        subprocess.check_call(
            ["cmake", ext.sourcedir] + cmake_args, cwd=self.build_temp
        )
        subprocess.check_call(
            ["cmake", "--build", ".", "--"] + build_args, cwd=self.build_temp
        )


setup(
    name="pycoacd",
    version=os.getenv("PYCOACD_VERSION"),
    python_requires=">=3.6",
    ext_modules=[CMakeExtension("pycoacd")],
    cmdclass={"build_ext": CMakeBuild},
    zip_safe=False,
    packages=["pycoacd-stubs"],
    package_dir={"pycoacd-stubs": "python/pycoacd-stubs"},
    package_data={"pycoacd-stubs": ["*.pyi", "py.typed"]},
)
