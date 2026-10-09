#!/bin/sh
# Builds the test suite of the pinned PsyZ and runs it. PsyZ is the
# replacement for Sony's library that the port links (see ../README.md).
#
# usage: port/tools/check_psyz.sh [--headless] [--pictures DIR] [BUILD_DIR]
#
# --headless builds SDL without window support, for a Linux machine that has
# no window, graphics or audio development files. Tests that compare drawn
# pictures cannot pass that way.
#
# --pictures DIR keeps the pictures that the suite writes when a comparison
# fails: they are moved to DIR. Without it they are removed.
#
# BUILD_DIR and DIR are taken from where the script is called; the default
# build folder is port/build/psyz-tests. The logs are written next to it.
#
# Needs git, a C and a C++ compiler, CMake 3.21 or later and Ninja.
# Prints the test program's count for each area of the suite and, last, for
# the whole suite. Status 0 when every area passes, 1 when one does not,
# 2 when nothing could be built or listed. check_psyz_controls.sh runs this
# script against stand-ins for git, CMake and the test program.
set -eu

headless=0
pictures=
while :; do
    case ${1:-} in
    --headless) headless=1; shift ;;
    --pictures)
        [ $# -ge 2 ] || { echo "--pictures needs a folder"; exit 2; }
        mkdir -p "$2" || { echo "cannot make $2"; exit 2; }
        pictures=$(cd "$2" && pwd)
        shift 2 ;;
    *) break ;;
    esac
done

port=$(cd "$(dirname "$0")/.." && pwd)
psyz=$port/external/psyz
# The script changes folder below, so the build folder is made absolute
# here. Making it also makes the folder that the logs go to.
build=${1:-$port/build/psyz-tests}
mkdir -p "$build" || { echo "cannot make $build"; exit 2; }
build=$(cd "$build" && pwd)

git -C "$port" submodule update --init external/psyz &&
    git -C "$psyz" submodule update --init --depth 1 external/SDL ||
    { echo "cannot fetch the submodule or the SDL source"; exit 2; }
echo "psyz at $(git -C "$psyz" rev-parse HEAD)"

set -- -GNinja -DGTE_USE_HW_SQRT=ON -DCMAKE_BUILD_TYPE=Debug
if [ $headless = 1 ]; then
    set -- "$@" -DSDL_UNIX_CONSOLE_BUILD=ON
    export SDL_AUDIODRIVER=dummy
fi
cmake "$@" -S "$psyz/psyz/tests" -B "$build" > "$build.configure.log" 2>&1 ||
    { echo "configure failed, see $build.configure.log"; exit 2; }
cmake --build "$build" > "$build.build.log" 2>&1 ||
    { echo "build failed, see $build.build.log"; exit 2; }
echo "built $build/psyz_tests"

# One run per area of the suite, so that each area's count is the test
# program's own. The suite reads its files from its folder and writes a
# picture next to the expected one when a comparison fails; those are
# moved away or removed afterwards so that the submodule stays clean.
status=0
if [ "$(uname)" = "Linux" ]; then export SDL_VIDEODRIVER=offscreen; fi
cd "$psyz/psyz/tests"
areas=$("$build/psyz_tests" --list 2>/dev/null | sed 's/::.*//' | sort -u)
[ -n "$areas" ] || { echo "the test program lists no tests"; exit 2; }
: > "$build.test.log"
for area in $areas; do
    "$build/psyz_tests" --filter="$area::*" --output=plain > "$build.area.log" 2>&1 || status=1
    cat "$build.area.log" >> "$build.test.log"
    echo "$area: $(tail -1 "$build.area.log")"
done
"$build/psyz_tests" --output=plain > "$build.area.log" 2>&1 || status=1
echo "all: $(tail -1 "$build.area.log")"
if [ -n "$pictures" ]; then
    for picture in expected/*.actual.png; do
        [ -f "$picture" ] && mv "$picture" "$pictures/"
    done
fi
rm -f "$build.area.log" expected/*.actual.png
exit $status
