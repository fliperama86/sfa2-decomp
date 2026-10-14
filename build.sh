#!/bin/sh
# Build the port's program for one system and, with -r, start it.
#
#   ./build.sh [-r] win|mac [PROGRAM OPTION ...]
#
#   win   port/build/host/sfa2.exe, the 32-bit Windows program. Needs i686-w64-mingw32-gcc, CMake (3.21 or later),
#         Ninja, git and Python 3.11 or later on PATH. On Linux and under WSL this is a cross build.
#   mac   port/build/mac/sfa2-mac, the program for macOS on Apple Silicon. Run it on such a Mac, with the tools that
#         port/mac/README.md names.
#   -r    after a build that ended well, start the program with the disc image that SFA2_DISC names (the .cue or
#         the .bin itself). The program gets --skip-programs, then --settings FILE (SFA2_SETTINGS, else
#         port/settings.ini) unless a PROGRAM OPTION is --settings, then the PROGRAM OPTIONS, then the disc. Its
#         status is this script's. Without -r a PROGRAM OPTION is refused.
#
# This script decides nothing about the build. It runs the tools of port/tools in the order of port/README.md
# ("Building it for the port", "Building it") and of port/mac/README.md, with those tools' own default folders:
#
#   win   psyzbuild.py                                    PsyZ and SDL, to port/build/psyz-i686-w64-mingw32
#         hostbuild.py --psyz port/build/psyz-i686-w64-mingw32   the program
#   mac   psyzbuild.py --cc clang --build port/build/psyz-mac
#         hostbuild.py --guest --psyz port/build/psyz-mac        the guest image
#         macbuild.py                                            the program
#
# What a second run does not do again:
#   - The PsyZ submodule is fetched only when port/external/psyz is empty (git submodule update --init, then its
#     SDL submodule). A checkout that is there is never moved; a line says so when it is not at the pinned commit.
#   - The library step is skipped when the last run of it by this script ended well for the same PsyZ commit with no
#     change in its work tree, the same port/psyz.patch, the same psyzbuild.py and the same first line of the
#     compiler's --version. That is written beside the build folder, as FOLDER.key. A PsyZ folder that is not a git
#     work tree of its own is always built again (psyzbuild.py itself compiles only what changed).
#   - The program step (hostbuild.py, and macbuild.py for the Mac) is skipped when the last run of it by this script
#     ended well and nothing it was made from changed since: git status shows no change and no new file under ps1/
#     and port/, the commit's trees of those two folders are the same, and so are this script and everything the
#     library step compares. That is written beside the program's folder, as FOLDER.key. In a tree with a change
#     there, and in a folder that is no git work tree, the step always runs, and it has no memory of its own: when
#     hostbuild.py runs, it compiles every unit.
#   With -r the program is started after a skipped step as after a run one.
#
# One machine's own values come from the environment or from the file local/build.env, which this script reads as
# shell text before anything else when it exists (the folder local/ is not in Git): additions to PATH, and
#   SFA2_DISC      the disc image for -r. For the Windows program under WSL, a path in Windows form, or a path of
#                  an existing file, which is turned into that form with wslpath.
#   SFA2_SETTINGS  the settings file for -r in place of port/settings.ini
#   SFA2_PYTHON    the Python to run the tools with, by default python3
#
# Status: 0 built (with -r, the program's status); 2 a wrong command line or -r without SFA2_DISC, before anything
# is built; else the status of the tool that failed. Reads no game file. port/tools/test_build_sh.py checks the
# order, the skipping and the program's command line with stand-ins for the tools, in a few seconds.
set -eu

usage() {
    echo "usage: ./build.sh [-r] win|mac [PROGRAM OPTION ...]" >&2
    exit 2
}

cd "$(dirname "$0")"
if [ -f local/build.env ]; then
    . ./local/build.env
fi
PYTHON=${SFA2_PYTHON:-python3}

run=0
target=
while [ $# -gt 0 ]; do
    case $1 in
        -r) run=1 ;;
        win|mac) target=$1; shift; break ;;
        *) usage ;;
    esac
    shift
done
[ -n "$target" ] || usage
if [ $run = 0 ] && [ $# -gt 0 ]; then
    echo "build.sh: $1: a program option needs -r" >&2
    usage
fi
if [ $run = 1 ] && [ -z "${SFA2_DISC:-}" ]; then
    echo "build.sh: -r needs SFA2_DISC, the .cue or .bin of your disc image (in the environment or in local/build.env)" >&2
    exit 2
fi

PSYZ=port/external/psyz
case $target in
    win) CC=i686-w64-mingw32-gcc; LIBRARY=port/build/psyz-i686-w64-mingw32; OUT=port/build/host; PROGRAM=$OUT/sfa2.exe ;;
    mac) CC=clang; LIBRARY=port/build/psyz-mac; OUT=port/build/mac; PROGRAM=$OUT/sfa2-mac ;;
esac

# The submodule: fetched when it is not there, left alone when it is.
if [ ! -d $PSYZ/psyz ]; then
    echo "build.sh: fetching the PsyZ submodule and its SDL"
    git submodule update --init $PSYZ
    git -C $PSYZ submodule update --init external/SDL
elif git submodule status $PSYZ 2>/dev/null | grep -q '^+'; then
    echo "build.sh: note: $PSYZ is not at the pinned commit (git submodule update $PSYZ moves it there)"
fi

# What the library was built from, or nothing when that cannot be told (then it is built again).
library_key() {
    top=$(git -C $PSYZ rev-parse --show-toplevel 2>/dev/null) || return 0
    [ "$top" = "$(cd $PSYZ && pwd -P)" ] || return 0
    [ -z "$(git -C $PSYZ status --porcelain)" ] || return 0
    version=$($CC --version 2>/dev/null | head -n 1)
    [ -n "$version" ] || return 0
    echo "psyz $(git -C $PSYZ rev-parse HEAD)"
    echo "patch $(git hash-object port/psyz.patch)"
    echo "tool $(git hash-object port/tools/psyzbuild.py)"
    echo "compiler $version"
}

key=$(library_key)
if [ -n "$key" ] && [ -f $LIBRARY/psyz.json ] && [ -f $LIBRARY.key ] && [ "$key" = "$(cat $LIBRARY.key)" ]; then
    echo "build.sh: the library is built already ($LIBRARY)"
else
    rm -f $LIBRARY.key
    "$PYTHON" port/tools/psyzbuild.py --cc $CC --build $LIBRARY
    if [ -n "$key" ]; then
        printf '%s\n' "$key" > $LIBRARY.key
    fi
fi

# What the program was built from, or nothing when that cannot be told (then it is built again).
program_key() {
    [ -n "$key" ] || return 0
    ps1=$(git rev-parse -q --verify HEAD:ps1 2>/dev/null) || return 0
    port=$(git rev-parse -q --verify HEAD:port 2>/dev/null) || return 0
    [ -z "$(git status --porcelain -- ps1 port)" ] || return 0
    printf '%s\n' "$key"
    echo "ps1 $ps1"
    echo "port $port"
    echo "script $(git hash-object build.sh)"
}

key=$(program_key)
if [ -n "$key" ] && [ -f $PROGRAM ] && [ -f $OUT.key ] && [ "$key" = "$(cat $OUT.key)" ]; then
    echo "build.sh: the program is built already ($PROGRAM)"
else
    rm -f $OUT.key
    case $target in
        win) "$PYTHON" port/tools/hostbuild.py --psyz $LIBRARY ;;
        mac) "$PYTHON" port/tools/hostbuild.py --guest --psyz $LIBRARY
             "$PYTHON" port/tools/macbuild.py ;;
    esac
    if [ -n "$key" ]; then
        printf '%s\n' "$key" > $OUT.key
    fi
fi
[ $run = 1 ] || exit 0

# A Windows program started from WSL takes its paths in Windows form.
native() {
    if [ $target = win ] && [ -e "$1" ] && command -v wslpath >/dev/null 2>&1; then
        wslpath -w "$1"
    else
        printf '%s\n' "$1"
    fi
}

settings=1
for option do
    [ "$option" != --settings ] || settings=0
done
if [ $settings = 1 ]; then
    set -- --settings "$(native "${SFA2_SETTINGS:-port/settings.ini}")" "$@"
fi
exec $PROGRAM --skip-programs "$@" "$(native "$SFA2_DISC")"
