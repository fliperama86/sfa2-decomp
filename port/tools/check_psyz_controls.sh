#!/bin/sh
# Controls for check_psyz.sh: its folders, its steps and its exit statuses,
# run against stand-ins for git, CMake and the test program in a scratch
# folder. Nothing is fetched and nothing is compiled.
#
# usage: port/tools/check_psyz_controls.sh [SCRIPT]
#
# SCRIPT is the script under control, by default the check_psyz.sh next to
# this file. Prints one line per case and, last, the count. Status 0 when
# every case is as expected, 1 otherwise.
set -u

here=$(cd "$(dirname "$0")" && pwd)
script=${1:-$here/check_psyz.sh}
case $script in /*) ;; *) script=$PWD/$script ;; esac
real_git=$(command -v git)

scratch=$(mktemp -d)
trap 'rm -rf "$scratch"' EXIT

# The stand-ins. STUB says which of them misbehaves and how.
mkdir "$scratch/bin"
cat > "$scratch/bin/git" <<'EOF'
#!/bin/sh
[ "$STUB" = fetch_fails ] && exit 1
case "$*" in *rev-parse*) echo 0000000000000000000000000000000000000000 ;; esac
exit 0
EOF
cat > "$scratch/bin/cmake" <<'EOF'
#!/bin/sh
echo "$*" >> "$STUB_LOG.cmake"
if [ "$1" = --build ]; then
    [ "$STUB" = build_fails ] && exit 1
    cp "$STUB_PROGRAM" "$2/psyz_tests"
    exit 0
fi
[ "$STUB" = configure_fails ] && exit 1
while [ $# -gt 1 ]; do
    if [ "$1" = -B ]; then mkdir -p "$2"; fi
    shift
done
exit 0
EOF
# Three tests in the areas "one" and "two". It notes the folder it is run
# from and leaves a picture there, as the suite does after a failed
# comparison.
cat > "$scratch/psyz_tests" <<'EOF'
#!/bin/sh
pwd >> "$STUB_LOG.cwd"
mkdir -p expected && : > expected/stand-in.actual.png
filter=
for a in "$@"; do
    case $a in
    --list)
        [ "$STUB" = lists_nothing ] || printf 'one::a\none::b\ntwo::c\n'
        exit 0 ;;
    --filter=*) filter=${a#--filter=} ;;
    esac
done
bad=0
case "$STUB:$filter" in area_fails:two* | area_fails: | whole_fails:) bad=1 ;; esac
case $filter in
'one::*') echo "ztest: 2 passed, 0 failed, 0 skipped" ;;
'two::*') echo "ztest: $((1 - bad)) passed, $bad failed, 0 skipped" ;;
'') echo "ztest: $((3 - bad)) passed, $bad failed, 0 skipped" ;;
*) echo "the stand-in does not know the filter $filter"; exit 3 ;;
esac
exit $bad
EOF
chmod +x "$scratch/bin/git" "$scratch/bin/cmake" "$scratch/psyz_tests"

cases=0
good=0
trees=0

# tree: a checkout as a fresh clone has it, with no build folder.
tree() {
    trees=$((trees + 1))
    root=$scratch/tree$trees
    tests=$root/port/external/psyz/psyz/tests
    mkdir -p "$root/port/tools" "$tests"
    cp "$script" "$root/port/tools/check_psyz.sh"
}

# run STUB FOLDER ARGS...: the script of the current tree, called directly
# from FOLDER. Sets out and st.
run() {
    stub=$1
    from=$2
    shift 2
    out=$(cd "$from" && STUB=$stub STUB_LOG=$root/log \
        STUB_PROGRAM=$scratch/psyz_tests PATH="$scratch/bin:$PATH" \
        "$root/port/tools/check_psyz.sh" "$@" 2>&1)
    st=$?
}

# expect DESCRIPTION CONDITION...: one case.
expect() {
    what=$1
    shift
    cases=$((cases + 1))
    if "$@"; then
        good=$((good + 1))
        echo "ok $cases: $what"
    else
        echo "FAIL $cases: $what (status $st; last line: $(printf '%s\n' "$out" | tail -1))"
    fi
}

passes() { [ "$st" = 0 ] && [ "$(printf '%s\n' "$out" | tail -1)" = "all: ztest: 3 passed, 0 failed, 0 skipped" ]; }
status_is() { [ "$st" = "$1" ]; }
says() { printf '%s\n' "$out" | grep -q -F -x "$1"; }

tree
run fine "$root"
default_ok() { passes && [ -x "$root/port/build/psyz-tests/psyz_tests" ] && [ -f "$root/port/build/psyz-tests.configure.log" ]; }
expect "fresh checkout, default folder, called from the root" default_ok

tree
run fine "$scratch"
expect "fresh checkout, default folder, called from elsewhere" default_ok

tree
mkdir "$root/work"
run fine "$root/work" out/b
relative_ok() { passes && [ -x "$root/work/out/b/psyz_tests" ] && [ -f "$root/work/out/b.test.log" ]; }
expect "relative folder that does not exist yet" relative_ok
ran_in_suite() { [ "$(sort -u "$root/log.cwd" 2>/dev/null)" = "$tests" ]; }
expect "the test program is run from the folder of the suite" ran_in_suite
no_pictures() { [ -s "$root/log.cwd" ] && [ -d "$tests/expected" ] && [ -z "$(ls "$tests/expected")" ]; }
expect "pictures that the suite leaves are removed" no_pictures
no_switch() { [ -s "$root/log.cmake" ] && ! grep -q SDL_UNIX_CONSOLE_BUILD "$root/log.cmake"; }
expect "without --headless SDL is built with window support" no_switch

tree
run fine "$root" "$root/abs"
absolute_ok() { passes && [ -x "$root/abs/psyz_tests" ]; }
expect "absolute folder" absolute_ok

tree
run fine "$root" --headless
headless_ok() { passes && grep -q -e '-DSDL_UNIX_CONSOLE_BUILD=ON' "$root/log.cmake" 2>/dev/null; }
expect "--headless builds SDL without window support" headless_ok

tree
run area_fails "$root"
area_ok() { status_is 1 && says "one: ztest: 2 passed, 0 failed, 0 skipped" && says "two: ztest: 0 passed, 1 failed, 0 skipped"; }
expect "an area that fails gives status 1 and its own line" area_ok

tree
run whole_fails "$root"
whole_ok() { status_is 1 && says "all: ztest: 2 passed, 1 failed, 0 skipped"; }
expect "the whole suite failing while each area passes gives status 1" whole_ok

tree
run lists_nothing "$root"
expect "a test program that lists nothing gives status 2" status_is 2

tree
run configure_fails "$root"
expect "a configure step that fails gives status 2" status_is 2

tree
run build_fails "$root"
expect "a build that fails gives status 2" status_is 2

tree
run fetch_fails "$root"
expect "a fetch that fails gives status 2" status_is 2

# A fresh checkout takes the mode of a file from Git, not from this folder.
out=$("$real_git" -C "$here" ls-files -s check_psyz.sh check_psyz_controls.sh 2>&1)
st=$?
modes_ok() { [ "$(printf '%s\n' "$out" | cut -c1-6 | sort -u)" = 100755 ] && [ "$(printf '%s\n' "$out" | wc -l)" -eq 2 ]; }
expect "both scripts are executable in Git" modes_ok

echo "controls: $good of $cases as expected"
[ "$good" = "$cases" ]
