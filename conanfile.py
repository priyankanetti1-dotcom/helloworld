import os

from conan import ConanFile
from conan.tools.files import copy
from conan.tools.gnu import MakeDeps


class HelloWorldConan(ConanFile):
    name = "helloworld"
    version = "0.1.0"
    description = "Hello world sample built with GNU make and tested with GoogleTest"
    license = "MIT"

    settings = "os", "arch", "compiler", "build_type"
    options = {
        "run_build": [True, False], 
        "run_tests": [True, False]
        }
    default_options = {
        "run_build": True, 
        "run_tests": False
        }

    exports_sources = "Makefile", "include/*", "src/*", "tests/*"

    def requirements(self):
        self.requires("gtest/1.14.0")

    def layout(self):
        self.folders.source = "."
        self.folders.build = "."
        self.folders.generators = os.path.join("build", "conan")

    def generate(self):
        MakeDeps(self).generate()

    def _make_args(self):
        args = []
        compilers = self.conf.get(
            "tools.build:compiler_executables", default={}, check_type=dict
        )
        if "cpp" in compilers:
            args.append('CXX="{}"'.format(compilers["cpp"]))
        if "c" in compilers:
            args.append('CC="{}"'.format(compilers["c"]))
        return " ".join(args)

    def build(self):
        jobs = os.cpu_count() or 1
        args = self._make_args()
        if self.options.run_build:
            self.run("make build -j{} {}".format(jobs, args), cwd=self.build_folder)
        if self.options.run_tests:
            self.run(
                'make test -j{} {} GTEST_FLAGS="--gtest_output=xml:build/test-results.xml"'.format(
                    jobs, args
                ),
                cwd=self.build_folder,
            )

    def package(self):
        copy(
            self,
            "helloworld",
            src=os.path.join(self.build_folder, "build", "bin"),
            dst=os.path.join(self.package_folder, "bin"),
        )

    def package_info(self):
        self.cpp_info.bindirs = ["bin"]
        self.cpp_info.libdirs = []
        self.cpp_info.includedirs = []
