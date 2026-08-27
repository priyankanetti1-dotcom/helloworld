# ---------------------------------------------------------------------------
# Toolchain
# ---------------------------------------------------------------------------
# Use the standard Ubuntu developer toolchain by default, while still allowing
# overrides like GCC_HOME=/usr/local or CXX=/usr/bin/clang++.
GCC_HOME ?= /usr

ifneq ($(wildcard $(GCC_HOME)/bin/g++),)
  # Only take over CXX when it still holds make's built-in default.
  ifeq ($(origin CXX),default)
    CXX := $(GCC_HOME)/bin/g++
  endif
  ifeq ($(origin CC),default)
    CC := $(GCC_HOME)/bin/gcc
  endif
  GCC_LIBDIR := $(firstword $(wildcard $(GCC_HOME)/lib64 $(GCC_HOME)/lib))
  ifneq ($(GCC_LIBDIR),)
    LDFLAGS += -L$(GCC_LIBDIR) -Wl,-rpath,$(GCC_LIBDIR)
  endif
endif

AR       ?= ar
CXXSTD   ?= -std=c++17
CXXFLAGS ?= $(CXXSTD) -g -O0 -Wall -Wextra
CPPFLAGS += -Iinclude
LDLIBS   += -lpthread

# ---------------------------------------------------------------------------
# Dependencies (Google Test) via Conan
# ---------------------------------------------------------------------------
# conanfile.py's MakeDeps generator emits build/conan/conandeps.mk, which make
# pulls in below. If conan is not on PATH the build falls back to a system gtest.
CONAN          ?= $(firstword $(wildcard .venv/bin/conan) conan)
CONAN_DIR      := build/conan
CONAN_DEPS_MK  := $(CONAN_DIR)/conandeps.mk
CONAN_PROFILE  ?= ./conan/profiles/gcc13
CONAN_FLAGS    ?= --build=missing
CONAN_BIN      := $(shell command -v $(CONAN) 2>/dev/null)

ifneq ($(CONAN_BIN),)
  # Including the fragment makes `conan install` run on demand.
  ifeq ($(filter clean distclean,$(MAKECMDGOALS)),)
    -include $(CONAN_DEPS_MK)
  endif
endif

ifdef CONAN_LIBS
  GTEST_CPPFLAGS ?= $(addprefix -I,$(CONAN_INCLUDE_DIRS)) $(addprefix -D,$(CONAN_DEFINES))
  GTEST_LDFLAGS  ?= $(addprefix -L,$(CONAN_LIB_DIRS))
  GTEST_LIBS     ?= $(addprefix -l,$(CONAN_LIBS)) $(addprefix -l,$(CONAN_SYSTEM_LIBS))
else
  # Fallback: a system- or manually-installed gtest. Override as needed, e.g.
  #   make test GTEST_CPPFLAGS=-I/opt/gtest/include GTEST_LDFLAGS=-L/opt/gtest/lib
  GTEST_CPPFLAGS ?=
  GTEST_LDFLAGS  ?=
  GTEST_LIBS     ?= -lgtest -lgtest_main
endif

BUILD_DIR := build
OBJ_DIR   := $(BUILD_DIR)/obj
BIN_DIR   := $(BUILD_DIR)/bin

LIB_SRCS  := $(filter-out src/main.cpp,$(wildcard src/*.cpp))
LIB_OBJS  := $(patsubst src/%.cpp,$(OBJ_DIR)/src/%.o,$(LIB_SRCS))
MAIN_OBJ  := $(OBJ_DIR)/src/main.o
LIB       := $(BUILD_DIR)/libhelloworld.a

TEST_SRCS := $(wildcard tests/*.cpp)
TEST_OBJS := $(patsubst tests/%.cpp,$(OBJ_DIR)/tests/%.o,$(TEST_SRCS))

APP       := $(BIN_DIR)/helloworld
TEST_BIN  := $(BIN_DIR)/helloworld_tests

DEPS := $(LIB_OBJS:.o=.d) $(MAIN_OBJ:.o=.d) $(TEST_OBJS:.o=.d)

.PHONY: all build test run deps clean distclean toolchain

all: build

build: $(APP)

deps: $(CONAN_DEPS_MK)

$(CONAN_DEPS_MK): conanfile.py
	@test -n "$(CONAN_BIN)" || { echo "conan not found on PATH (pip install conan)"; exit 1; }
	$(CONAN) install . --profile:all=$(CONAN_PROFILE) $(CONAN_FLAGS)

toolchain:
	@echo "CXX            = $(CXX)"
	@$(CXX) --version | head -1
	@echo "GTEST_CPPFLAGS = $(GTEST_CPPFLAGS)"
	@echo "GTEST_LDFLAGS  = $(GTEST_LDFLAGS)"
	@echo "GTEST_LIBS     = $(GTEST_LIBS)"

$(APP): $(MAIN_OBJ) $(LIB) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) $(LDLIBS) -o $@

$(LIB): $(LIB_OBJS)
	$(AR) rcs $@ $^

$(OBJ_DIR)/src/%.o: src/%.cpp | $(OBJ_DIR)/src
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OBJ_DIR)/tests/%.o: tests/%.cpp | $(OBJ_DIR)/tests
	$(CXX) $(CPPFLAGS) $(GTEST_CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(TEST_BIN): $(TEST_OBJS) $(LIB) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) $(GTEST_LDFLAGS) $(GTEST_LIBS) $(LDLIBS) -o $@

test: $(TEST_BIN)
	$(TEST_BIN) $(GTEST_FLAGS)

run: $(APP)
	$(APP)

$(BIN_DIR) $(OBJ_DIR)/src $(OBJ_DIR)/tests:
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(LIB)

distclean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)
