# HelloWorld

A small C++17 "hello world" project built with GNU make and unit tested with
GoogleTest. It is intentionally seeded with **failing unit tests** and
**static-analysis (Coverity) defects** so it can be used as a playground for
automated remediation agents.

## Layout

```
include/                   public headers
src/                       library sources + main.cpp
tests/                     GoogleTest unit tests
Makefile                   GNU make build
conanfile.py               Conan recipe (deps + build/test methods)
conan/profiles/gcc13       Conan profile pinned to /depot/gcc-13.3.0
.github/workflows/ci.yml   CI: conan install / conan build
```

## Toolchain

The Makefile defaults `CXX` to `$(GCC_HOME)/bin/g++` with `GCC_HOME=/depot/gcc-13.3.0`,
and adds `-L`/`-rpath` for that toolchain's `libstdc++`. It falls back to the
system `g++` when that path does not exist. Override either way:

```sh
make build GCC_HOME=/usr
make build CXX=/usr/bin/clang++
make toolchain      # print the resolved compiler and gtest flags
```

## Dependencies

GoogleTest is managed by Conan 2. `conanfile.py` uses the `MakeDeps` generator,
which writes `build/conan/conandeps.mk`; the Makefile includes that fragment and
derives `GTEST_CPPFLAGS` / `GTEST_LDFLAGS` / `GTEST_LIBS` from it.

```sh
pip install "conan>=2.0"
conan install . --profile:all=./conan/profiles/gcc13 --build=missing
conan build   . --profile:all=./conan/profiles/gcc13 -o "&:run_tests=True"
```

`conan build` runs `make build`, and additionally `make test` when the
`run_tests` option is enabled (default `False`).

To reproduce the CI pipeline locally:

```sh
./scripts/ci-local.sh                        # ./conan/profiles/gcc13
CONAN_PROFILE=default ./scripts/ci-local.sh  # conan's detected profile
RUN_TESTS=False ./scripts/ci-local.sh        # build only
PYTHON_BIN=/usr/bin/python3 ./scripts/ci-local.sh
```

Like CI, the script creates `.venv` with `/depot/Python/Python-3.11.2/bin/python3`
and installs Conan into it. The Makefile picks up `.venv/bin/conan` automatically.

If conan is not on `PATH`, the Makefile falls back to a system/manual gtest:

```sh
make test GTEST_CPPFLAGS=-I/path/to/gtest/include GTEST_LDFLAGS=-L/path/to/gtest/lib
```

## Build and run with make directly

```sh
make deps           # conan install (only needed once)
make build          # -> build/bin/helloworld
make run
make test           # -> build/bin/helloworld_tests
make clean          # objects/binaries, keeps build/conan
make distclean      # everything under build/
```

## Known failing tests

4 of the 18 tests fail on purpose until the underlying bugs are fixed:

| Test | Bug |
| --- | --- |
| `GreeterTest.GreetingEndsWithAnExclamationMark` | `Greeter::greet()` omits the trailing `!` |
| `TextUtilsTest.ReverseWordsKeepsEveryWord` | `reverseWords()` loop is off by one and drops the first word |
| `TextUtilsTest.ReverseWordsOfASingleWordIsTheWordItself` | same off-by-one; a one word sentence comes back empty |
| `StatsTest.MedianOfAnEvenNumberOfElements` | `median()` never averages the two middle elements |

## Seeded Coverity-style defects

| File | Function | Checker |
| --- | --- | --- |
| [src/greeter.cpp](src/greeter.cpp) | `Greeter::Greeter()` | `UNINIT_CTOR` — `greetCount_` never initialized |
| [src/text_utils.cpp](src/text_utils.cpp) | `duplicateUpper()` | `RESOURCE_LEAK` — early return leaks the malloc'd buffer |
| [src/text_utils.cpp](src/text_utils.cpp) | `upperLength()` | `USE_AFTER_FREE` — buffer read after `free()` |
| [src/text_utils.cpp](src/text_utils.cpp) | `extractDomain()` | `NULL_RETURNS` — `strchr()` result dereferenced unchecked |
| [src/text_utils.cpp](src/text_utils.cpp) | `renderBanner()` | `STRING_OVERFLOW` — unbounded `strcpy()` into a 32 byte array |
| [src/stats.cpp](src/stats.cpp) | `mean()` | `DIVIDE_BY_ZERO` — empty input |
| [src/stats.cpp](src/stats.cpp) | `maxValue()` | `UNINIT` — `best` read before assignment on empty input |
| [src/main.cpp](src/main.cpp) | `main()` | `RESOURCE_LEAK` — `shout` is never freed |
