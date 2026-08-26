#!/usr/bin/env bash
#
# Run locally what .github/workflows/ci.yml runs in GitHub Actions.
#
#   ./scripts/ci-local.sh                       # use ./conan/profiles/gcc13
#   CONAN_PROFILE=default ./scripts/ci-local.sh # use conan's detected profile
#   RUN_TESTS=False ./scripts/ci-local.sh       # build only
#   PYTHON_BIN=/usr/bin/python3 ./scripts/ci-local.sh
#
# The unit tests are expected to fail, so a non-zero exit code is normal.

set -uo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$REPO_ROOT"

PYTHON_BIN="${PYTHON_BIN:-/depot/Python/Python-3.11.2/bin/python3}"
VENV_DIR="${VENV_DIR:-$REPO_ROOT/.venv}"
CONAN_VERSION="${CONAN_VERSION:-2.22.0}"
CONAN_PROFILE="${CONAN_PROFILE:-./conan/profiles/gcc13}"
RUN_TESTS="${RUN_TESTS:-True}"

step() { printf '\n\033[1;34m==> %s\033[0m\n' "$*"; }
fail() { printf '\033[1;31m%s\033[0m\n' "$*" >&2; exit 1; }

step "Environment"
make --version | head -1
[[ -x "$PYTHON_BIN" ]] || fail "python not found at $PYTHON_BIN (override with PYTHON_BIN=...)"
"$PYTHON_BIN" --version

step "Create Python virtualenv"
if [[ ! -x "$VENV_DIR/bin/python" ]]; then
  "$PYTHON_BIN" -m venv "$VENV_DIR" || fail "failed to create venv at $VENV_DIR"
fi
export PATH="$VENV_DIR/bin:$PATH"
which python

step "Install Conan"
python -m pip install --upgrade pip --quiet || fail "pip upgrade failed"
python -m pip install "conan==$CONAN_VERSION" --quiet || fail "conan install failed"
conan --version

if [[ "$CONAN_PROFILE" == "default" && ! -f "$(conan config home)/profiles/default" ]]; then
  step "Detect Conan profile"
  conan profile detect --force
fi

step "Clean workspace"
make distclean

step "conan install"
conan install . --profile:all="$CONAN_PROFILE" --build=missing -o "&:run_tests=$RUN_TESTS" \
  || fail "conan install failed"

step "conan build (make build + gtest)"
conan build . --profile:all="$CONAN_PROFILE" -o "&:run_tests=$RUN_TESTS"
BUILD_STATUS=$?

step "Run the application"
if [[ -x build/bin/helloworld ]]; then
  ./build/bin/helloworld
else
  fail "build/bin/helloworld was not produced"
fi

step "Summary"
if [[ -f build/test-results.xml ]]; then
  echo "test report: build/test-results.xml"
  grep -o 'tests="[0-9]*" failures="[0-9]*"' build/test-results.xml | head -1
fi

if [[ $BUILD_STATUS -ne 0 ]]; then
  printf '\033[1;33mconan build exited %d (expected while the seeded bugs are unfixed)\033[0m\n' "$BUILD_STATUS"
fi
exit $BUILD_STATUS
