#!/usr/bin/env python3
"""Controls for families.py using synthetic inputs.

Builds small fake PS-X executables, build configurations and family tables in
a temporary directory. No game data is involved. The expected output is worked
out here from each layout, not read from the tool.
"""

from __future__ import annotations

import collections
import os
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

from test_disc_tools import make_archive, make_program

TOOLS = Path(__file__).resolve().parent
OPEN, CLOSE, RETURN, ONE = 0x27BDFFE8, 0x27BD0018, 0x03E00008, 0x24020001
STUB_JUMP, JALR_T9, JR_T9 = 0x01400008, 0x0320F809, 0x03200008
UNIDENTIFIED = "unidentified"
START = 0x80010000
HEADER = " family            library functions  called directly by  reached by"


def tool(*args, seed: str | None = None) -> subprocess.CompletedProcess:
    env = dict(os.environ, PYTHONHASHSEED=seed) if seed is not None else None
    return subprocess.run([sys.executable, str(TOOLS / "families.py"), *map(str, args)], capture_output=True, text=True, env=env)


def jal(target: int) -> int:
    return 0x0C000000 | target >> 2 & 0x03FFFFFF


def jump(target: int) -> int:
    return 0x08000000 | target >> 2 & 0x03FFFFFF


def stub(table: int, number: int, ori: bool = False) -> list[int]:
    """`li t2,table`, `jr t2`, and `li t1,number` in the delay slot."""
    return [0x240A0000 | table, STUB_JUMP, (0x3409 if ori else 0x2409) << 16 | number]


def executable(words: list[int], start: int = START) -> bytes:
    body = struct.pack(f"<{len(words)}I", *words)
    header = bytearray(0x800)
    header[:8] = b"PS-X EXE"
    struct.pack_into("<II", header, 0x18, start, len(body))
    return bytes(header) + body


def lay_words(entries: list, start: int = START) -> tuple[list[int], dict[str, int], dict[str, int]]:
    """Place functions one after another. Each entry is (name, body), the body a function of the address book.

    Returns the words, the address of each function and its size in bytes.
    """
    sizes = {name: 4 * len(body(collections.defaultdict(int))) for name, body in entries}
    book, cursor = {}, start
    for name, _ in entries:
        book[name], cursor = cursor, cursor + sizes[name]
    return [word for _, body in entries for word in body(book)], book, sizes


def lay_out(entries: list, start: int = START) -> tuple[bytes, dict[str, int], dict[str, int]]:
    """The same, as an executable."""
    words, book, sizes = lay_words(entries, start)
    return executable(words, start), book, sizes


def config_text(units: list[tuple[str, str | None, list[tuple[str | None, object]]]]) -> str:
    """A build configuration: units of (source, image or None, functions of (name or None, address))."""
    text = ""
    for source, image, functions in units:
        text += f'[[unit]]\nsource = "{source}"\n' + (f'image = "{image}"\n' if image else "")
        entries = []
        for name, address in functions:
            shown = f'"{address}"' if isinstance(address, str) else str(address)
            entries.append("{ " + (f'name = "{name}", ' if name else "") + f"address = {shown} }}")
        text += "functions = [" + ", ".join(entries) + "]\n\n"
    return text


def verdict(ok: bool, detail: str) -> subprocess.CompletedProcess:
    return subprocess.CompletedProcess([], 0 if ok else 1, "as required" if ok else "", "" if ok else detail)


def quiet(proc: subprocess.CompletedProcess) -> subprocess.CompletedProcess:
    """The same run, but a traceback turns it into a failure of the case."""
    if "Traceback" in proc.stderr:
        return subprocess.CompletedProcess(proc.args, 99, proc.stdout, proc.stderr)
    return proc


def report(start, end, games, libs, outside, show=10) -> str:
    """The summary that a sweep must print.

    games: (direct, reached, through a register, elsewhere, open) per game function.
    libs: (family, rule that gave it) of each library function; a bare UNIDENTIFIED stands for (UNIDENTIFIED, "-").
    outside: declared addresses that are no start.
    """
    libs = [(f, "-") if f == UNIDENTIFIED else f for f in libs]
    assert all(isinstance(f, tuple) for f in libs)
    rules = collections.Counter(rule for _, rule in libs)
    libs = [family for family, _ in libs]
    lines = [
        f"swept {start:#x} to {end:#x}: {len(games) + len(libs)} functions, {len(games)} game and {len(libs)} library",
        f"functions of the build that are no start of the sweep: {len(outside)}",
        *[f"  {address:#x}" for address in sorted(outside)[:show]],
        HEADER,
    ]
    members = collections.Counter(libs)
    calling = collections.Counter(name for direct, *_ in games for name in direct)
    reaching = collections.Counter(name for _, reached, *_ in games for name in reached)
    for name in sorted(members, key=lambda n: (n == UNIDENTIFIED, n)):
        lines.append(f" {name:<17} {members[name]:>17} {calling[name]:>19} {reaching[name]:>11}")
    none = [game for game in games if not game[1]]
    lines += [
        f"library functions with a family by name: {rules['name']}, by BIOS call: {rules['bios']},"
        f" by folder: {rules['folder']}, by place: {rules['place']}",
        f"game functions that call a library function directly: {sum(1 for game in games if game[0])}",
        f"game functions that reach no library function: {len(none)};"
        f" closed: {sum(1 for game in none if not game[4])}, open: {sum(1 for game in none if game[4])}",
        f"game functions with a call through a register: {sum(1 for game in games if game[2])};"
        f" with a call elsewhere: {sum(1 for game in games if game[3])}",
    ]
    return "".join(line + "\n" for line in lines)


def joined(names) -> str:
    return ",".join(sorted(names)) or "-"


def exact(proc: subprocess.CompletedProcess, want: str) -> subprocess.CompletedProcess:
    return verdict(proc.returncode == 0 and proc.stdout == want, f"exit {proc.returncode}\n--- got\n{proc.stdout}--- wanted\n{want}{proc.stderr}")


def main_cases(root: Path):
    """A sweep over a rich layout: game functions that call each other and library functions of every family."""
    A, B = 0xA0, 0xB0
    plain = [OPEN, ONE, RETURN, CLOSE]
    entries = [
        # Game functions. The callers of a chain are at lower addresses than their callees.
        ("top3", lambda a: [OPEN, jal(a["mid3"]), 0, RETURN, CLOSE]),
        ("mid3", lambda a: [OPEN, jal(a["lb"]), 0, jal(a["leaf3"]), 0, RETURN, CLOSE]),
        ("leaf3", lambda a: [OPEN, jal(a["la1"]), 0, RETURN, CLOSE]),
        ("ping", lambda a: [OPEN, jal(a["lg"]), 0, jal(a["pong"]), 0, RETURN, CLOSE]),
        ("pong", lambda a: [OPEN, jal(a["ld"]), 0, jal(a["ping"]), 0, RETURN, CLOSE]),
        ("self", lambda a: [OPEN, jal(a["self"]), 0, jal(a["ld"]), 0, RETURN, CLOSE]),
        ("calls_next", lambda a: [OPEN, jal(a["next_fn"]), 0, RETURN, CLOSE]),  # the callee starts where it ends
        ("next_fn", lambda a: [OPEN, jal(a["lb"]), 0, RETURN, CLOSE]),
        ("j_call", lambda a: [OPEN, jump(a["next_fn"]), 0, RETURN, CLOSE]),
        # a `j` to the last word of the function before it, and to the middle of a function far before it
        ("j_mid", lambda a: [OPEN, jump(a["j_mid"] - 4), 0, jump(a["leaf3"] + 4), 0, RETURN, CLOSE]),
        # `j` to the second word, the first word and the `jr ra` of the function itself
        ("loop", lambda a: [OPEN, ONE, jump(a["loop"] + 4), 0, jump(a["loop"]), 0, jump(a["loop"] + 24), 0, RETURN, CLOSE]),
        ("far", lambda a: [OPEN, jal(0x80400000), 0, jal(0x80400000), 0, RETURN, CLOSE]),
        ("reg", lambda a: [OPEN, JALR_T9, 0, jal(a["la1"]), 0, JALR_T9, 0, jal(a["leafc"]), 0, RETURN, CLOSE]),
        ("leafc", lambda a: plain),
        ("jr_only", lambda a: [OPEN, JR_T9, 0, RETURN, CLOSE]),
        ("open_top", lambda a: [OPEN, jal(a["open_mid"]), 0, RETURN, CLOSE]),
        ("open_mid", lambda a: [OPEN, jal(a["open_src"]), 0, RETURN, CLOSE]),
        ("open_src", lambda a: [OPEN, JALR_T9, 0, RETURN, CLOSE]),
        ("twice", lambda a: [OPEN, jal(a["la1"]), 0, jal(a["la1"]), 0, RETURN, CLOSE]),
        ("calls_lib", lambda a: [OPEN, jal(a["lcalls"]), 0, RETURN, CLOSE]),
        # two library functions of one family are one direct family
        ("misc", lambda a: [OPEN, jal(a["lz"]), 0, jal(a["lo"]), 0, jal(a["lu"]), 0, jal(a["la1"]), 0, RETURN, CLOSE]),
        ("delay", lambda a: [OPEN, RETURN, jal(a["lg"])]),  # the call is the delay slot, the last word
        ("first_call", lambda a: [jal(a["lb"]), 0, RETURN, CLOSE]),  # the call is the first word
        # Library functions. The first one starts at the limit.
        ("lz", lambda a: stub(B, 5)),  # name, bios call and folder all apply: the name wins
        ("la1", lambda a: stub(A, 5)),  # a name that the table lacks, a bios call and a folder: the call wins
        ("ld", lambda a: plain),  # a folder only
        ("lu", lambda a: plain),  # nothing
        ("lb", lambda a: stub(B, 5)),
        ("lg", lambda a: stub(0xC0, 5)),
        ("lo", lambda a: stub(A, 0x3F, ori=True)),
        ("lx", lambda a: plain),  # sdk/x.c
        ("lop", lambda a: plain),  # other/gpu/d.c
        ("lspu", lambda a: plain),  # a folder that the table lacks
        ("lres", lambda a: plain),  # image = "resident"
        ("lother", lambda a: plain),  # declared by a unit of another image
        ("lt", lambda a: [0x240A00B0, STUB_JUMP, 0x24080005]),  # `li t0` instead of `li t1`
        ("lnojr", lambda a: [0x240A00B0, ONE, 0x24090005, RETURN, CLOSE]),
        ("lnop", lambda a: [0x240A00B0, STUB_JUMP, 0]),
        ("lmid", lambda a: [ONE, *stub(B, 5)]),  # the stub does not start the function
        ("lbad", lambda a: [0x240B00B0, STUB_JUMP, 0x24090005, RETURN, CLOSE]),  # `li t3`, not `li t2`
        ("lunk", lambda a: stub(B, 0x77)),  # a stub whose key the table lacks
        ("lcalls", lambda a: [OPEN, jal(a["lb"]), 0, RETURN, CLOSE]),
    ]
    exe_bytes, a, size = lay_out(entries)
    end = START + len(exe_bytes) - 0x800
    library = a["lz"]
    gnames = [name for name, _ in entries if a[name] < library]
    lnames = [name for name, _ in entries if a[name] >= library]
    S = lambda *names: set(names)  # noqa: E731
    # (direct, reached, through a register, elsewhere, open)
    game = {
        "top3": (S(), S("alpha", "beta"), 0, 0, False),
        "mid3": (S("beta"), S("alpha", "beta"), 0, 0, False),
        "leaf3": (S("alpha"), S("alpha"), 0, 0, False),
        "ping": (S("gamma"), S("gamma", "delta"), 0, 0, False),
        "pong": (S("delta"), S("gamma", "delta"), 0, 0, False),
        "self": (S("delta"), S("delta"), 0, 0, False),
        "calls_next": (S(), S("beta"), 0, 0, False),
        "next_fn": (S("beta"), S("beta"), 0, 0, False),
        "j_call": (S(), S("beta"), 0, 0, False),
        "j_mid": (S(), S(), 0, 2, True),
        "loop": (S(), S(), 0, 0, False),
        "far": (S(), S(), 0, 2, True),
        "reg": (S("alpha"), S("alpha"), 2, 0, True),
        "leafc": (S(), S(), 0, 0, False),
        "jr_only": (S(), S(), 0, 0, False),
        "open_top": (S(), S(), 0, 0, True),
        "open_mid": (S(), S(), 0, 0, True),
        "open_src": (S(), S(), 1, 0, True),
        "twice": (S("alpha"), S("alpha"), 0, 0, False),
        "calls_lib": (S(UNIDENTIFIED), S(UNIDENTIFIED), 0, 0, False),
        "misc": (S("alpha", UNIDENTIFIED, "zeta"), S("alpha", UNIDENTIFIED, "zeta"), 0, 0, False),
        "delay": (S("gamma"), S("gamma"), 0, 0, False),
        "first_call": (S("beta"), S("beta"), 0, 0, False),
    }
    family = {
        "lz": "zeta", "la1": "alpha", "ld": "delta", "lu": UNIDENTIFIED, "lb": "beta", "lg": "gamma", "lo": "alpha",
        "lx": UNIDENTIFIED, "lop": UNIDENTIFIED, "lspu": UNIDENTIFIED, "lres": "delta", "lother": UNIDENTIFIED,
        "lt": UNIDENTIFIED, "lnojr": UNIDENTIFIED, "lnop": UNIDENTIFIED, "lmid": UNIDENTIFIED, "lbad": UNIDENTIFIED,
        "lunk": UNIDENTIFIED, "lcalls": UNIDENTIFIED,
    }  # fmt: skip
    # The game functions that call each library function, each counted once however many calls it makes.
    rule = {
        "lz": "name", "la1": "bios", "ld": "folder", "lb": "bios", "lg": "bios", "lo": "bios", "lres": "folder",
    }  # every other function: "-"
    callers = {"la1": 4, "lb": 3, "lg": 2, "ld": 2, "lz": 1, "lo": 1, "lu": 1, "lcalls": 1}
    assert gnames == list(game) and lnames == list(family)

    named_game = {"top3": "top3", "loop": "loop_fn", "ping": "ping"}
    named_library = {"lz": "n_name", "la1": "other_name", "lx": "lx_name", "lres": "res_fn"}
    outside = [0x80000000, 0x80000004, 0x80000100, a["top3"] + 4, a["mid3"] + 8, a["loop"] + 12, a["lu"] + 4, a["lcalls"] + 8]
    outside += [START + len(exe_bytes) - 0x800 + 4 * n for n in range(4)]
    assert len(outside) == 12
    units = [
        ("sdk/gpu/a.c", None, [("n_name", a["lz"])]),
        ("sdk/gpu/b.c", None, [("other_name", a["la1"])]),
        ("sdk/gpu/c.c", None, [(None, a["ld"])]),
        ("sdk/x.c", None, [("lx_name", a["lx"])]),
        ("other/gpu/d.c", None, [(None, a["lop"])]),
        ("sdk/spu/e.c", None, [(None, a["lspu"])]),
        ("sdk/gpu/f.c", "resident", [("res_fn", a["lres"])]),
        ("sdk/gpu/g.c", "other", [("n_name", a["lother"]), (None, a["top3"] + 12)]),
        ("game/main.c", None, [("top3", a["top3"]), ("loop_fn", a["loop"]), ("ping", a["ping"]), (None, a["twice"])]),
        ("game/extra.c", None, [(None, address) for address in reversed(outside)]),
    ]
    config = root / "main.toml"
    config.write_text(config_text(units))
    table = root / "main-families.toml"
    table.write_text(
        '[names]\nn_name = "zeta"\nghost = "ghostfam"\n\n'
        '[bios]\n"a0:05" = "alpha"\n"b0:05" = "beta"\n"c0:05" = "gamma"\n"a0:3f" = "alpha"\n"b0:0a" = "ghostfam"\n\n'
        '[folders]\ngpu = "delta"\n"x.c" = "xfam"\nunused = "unusedfam"\n'
    )
    exe = root / "main.exe"
    exe.write_bytes(exe_bytes)
    base = [exe, "--config", config, "--families", table, "--library", f"{library:#x}", "--end", f"{end:#x}"]

    rows = [game[n] for n in gnames]
    libs = [(family[n], rule.get(n, "-")) for n in lnames]
    out, lib_out = root / "deep" / "er" / "game.tsv", root / "other" / "deeper" / "library.tsv"
    proc = tool(*base, "--out", out, "--library-out", lib_out)
    yield "summary-exact", exact(proc, report(START, end, rows, libs, outside)), 0, "as required"
    want_out = "".join(
        f"{a[n]:08x}\t{size[n]}\t{named_game.get(n, '-')}\t{joined(d)}\t{joined(r)}\t{reg}\t{other}\t{'open' if opened else 'closed'}\n"
        for n, (d, r, reg, other, opened) in game.items()
    )
    got = out.read_text() if out.exists() else None
    yield "out-rows", verdict(got == want_out, f"{got!r}\n{want_out!r}"), 0, "as required"
    want_lib = "".join(f"{a[n]:08x}\t{size[n]}\t{named_library.get(n, '-')}\t{family[n]}\t{callers.get(n, 0)}\t{rule.get(n, '-')}\n" for n in lnames)
    got = lib_out.read_text() if lib_out.exists() else None
    yield "library-out-rows", verdict(got == want_lib, f"{got!r}\n{want_lib!r}"), 0, "as required"
    only = tool(*base)
    yield "no-files-without-options", verdict(only.returncode == 0 and not (root / "game.tsv").exists(), only.stdout), 0, "as required"
    yield "show-three", exact(tool(*base, "--show", 3), report(START, end, rows, libs, outside, 3)), 0, "as required"
    yield "show-none", exact(tool(*base, "--show", 0), report(START, end, rows, libs, outside, 0)), 0, "as required"
    yield "show-more-than-found", exact(tool(*base, "--show", 40), report(START, end, rows, libs, outside, 40)), 0, "as required"
    # The sweep starts at the image start whether or not it is given.
    yield "start-given-as-default", exact(tool(*base, "--start", f"{START:#x}"), report(START, end, rows, libs, outside)), 0, "as required"
    # The family table lists the families that have a library function, not the ones that the table names.
    text = tool(*base).stdout
    yield "unused-family-not-listed", verdict("ghostfam" not in text and "xfam" not in text and "unusedfam" not in text, text), 0, "as required"


def mini_cases(root: Path):
    """A small layout for the range and the limit: two game functions, then two library functions."""
    plain = [OPEN, ONE, RETURN, CLOSE]
    entries = [
        ("f0", lambda a: plain),
        ("f1", lambda a: [OPEN, jal(a["f0"]), 0, jal(a["l"]), 0, RETURN, CLOSE]),
        ("l", lambda a: plain),
        ("m", lambda a: plain),
    ]
    exe_bytes, a, _ = lay_out(entries)
    end = START + len(exe_bytes) - 0x800
    exe = root / "mini.exe"
    exe.write_bytes(exe_bytes)
    config = root / "mini.toml"
    config.write_text(config_text([("game/m.c", None, [("f0", a["f0"]), ("m", a["m"])])]))
    table = root / "empty-families.toml"
    table.write_text("")  # every part may be absent

    def run(*extra, library=None, end_at=end):
        return tool(exe, "--config", config, "--families", table, "--library", f"{(a['l'] if library is None else library):#x}", "--end", f"{end_at:#x}", *extra)

    U = UNIDENTIFIED
    closed = (set(), set(), 0, 0, False)
    f1 = ({U}, {U}, 0, 0, False)
    yield "mini-whole", exact(run(), report(START, end, [closed, f1], [U, U], [])), 0, "as required"
    # From a start inside the image: the call to the function before it is a call elsewhere.
    cut = report(a["f1"], end, [({U}, {U}, 0, 1, True)], [U, U], [a["f0"]])
    yield "mini-start", exact(run("--start", f"{a['f1']:#x}"), cut), 0, "as required"
    yield "mini-end", exact(run(end_at=a["m"]), report(START, a["m"], [closed, f1], [U], [a["m"]])), 0, "as required"
    # A function that starts below the limit is a game function, whatever it holds.
    yield "mini-library-inside-function", exact(
        run(library=a["l"] + 4), report(START, end, [closed, (set(), set(), 0, 0, False), closed], [U], [])
    ), 0, "as required"
    yield "mini-library-at-first", exact(run(library=START), report(START, end, [], [U, U, U, U], [])), 0, "as required"
    # Both the start and the end may stand at the ends of the image.
    yield "mini-last-function-only", exact(
        run("--start", f"{a['m']:#x}"), report(a["m"], end, [], [U], [a["f0"]])
    ), 0, "as required"

    def refused(name: str, start, finish):
        extra = [] if start is None else ["--start", f"{start:#x}"]
        proc = run(*extra, end_at=finish)
        shown = START if start is None else start
        text = f"the range to sweep, {shown:#x} to {finish:#x}, is not a range of words inside the image"
        yield name, quiet(proc), 1, text

    yield from refused("range-start-zero", 0, end)
    yield from refused("range-end-zero", None, 0)
    yield from refused("range-start-zero-and-end-zero", 0, 0)
    yield from refused("range-end-past-image", None, end + 4)
    yield from refused("range-start-before-image", START - 4, end)
    yield from refused("range-start-unaligned", START + 2, end)
    yield from refused("range-end-unaligned", None, end - 2)
    yield from refused("range-empty", START + 8, START + 8)
    yield from refused("range-empty-at-end", end, end)
    yield from refused("range-backwards", START + 12, START + 8)
    yield from refused("range-start-at-image-end", end, end + 4)


def table_cases(root: Path):
    """The cases of the bios keys at the end of an image, the family table and the configuration that are refused."""
    # A stub in the last three words of the image has its key. One cut short by the end of the image has none.
    for label, tail, family in (("whole", stub(0xA0, 5), "alpha"), ("cut", stub(0xA0, 5)[:2], UNIDENTIFIED)):
        entries = [("g", lambda a: [OPEN, jal(a["s"]), 0, RETURN, CLOSE]), ("s", lambda a, t=tail: t)]
        exe_bytes, a, _ = lay_out(entries)
        end = START + len(exe_bytes) - 0x800
        exe = root / f"stub-{label}.exe"
        exe.write_bytes(exe_bytes)
        config = root / "empty.toml"
        config.write_text("")
        table = root / "alpha.toml"
        table.write_text('[bios]\n"a0:05" = "alpha"\n')
        proc = tool(exe, "--config", config, "--families", table, "--library", f"{a['s']:#x}", "--end", f"{end:#x}")
        direct = {family}
        yield f"stub-at-end-{label}", exact(proc, report(START, end, [(direct, direct, 0, 0, False)], [(family, "bios" if family == "alpha" else "-")], [])), 0, "as required"

    exe = root / "plain.exe"
    code = [OPEN, ONE, RETURN, CLOSE, OPEN, ONE, RETURN, CLOSE]
    exe.write_bytes(executable(code))
    end = START + 4 * len(code)
    config = root / "good.toml"
    config.write_text(config_text([("game/a.c", None, [("f", START)])]))
    table = root / "good-families.toml"
    table.write_text('[folders]\ngpu = "delta"\n')

    def run(config_path=None, table_path=None, executable_path=None, *extra):
        return quiet(
            tool(
                executable_path or exe, "--config", config_path or config, "--families", table_path or table,
                "--library", f"{START + 16:#x}", "--end", f"{end:#x}", *extra,
            )
        )

    yield "good-baseline", run(), 0, "swept"

    # An address below 0x10000000 still has eight digits in a row.
    low = root / "low.exe"
    low.write_bytes(executable([OPEN, ONE, RETURN, CLOSE], 0x00010000))
    rows = root / "low.tsv"
    proc = quiet(tool(low, "--config", config, "--families", table, "--library", "0x00010010", "--end", "0x00010010", "--out", rows))
    got = rows.read_text() if rows.exists() else None
    yield "out-address-has-eight-digits", verdict(proc.returncode == 0 and got == "00010000\t16\t-\t-\t-\t0\t0\tclosed\n", f"{proc.stdout}{got!r}"), 0, "as required"

    def bad_table(name: str, text: str, message: str):
        path = root / f"table-{name}.toml"
        path.write_text(text)
        yield f"table-{name}", run(table_path=path), 1, message

    yield from bad_table("unknown-key", '[names]\na = "b"\n\n[other]\nx = "y"\n', "unknown key `other`")
    # The first unknown key in order, whatever the order in which the keys of a set come out.
    path = root / "table-many-unknown.toml"
    path.write_text("".join(f'[{key}]\nx = "y"\n' for key in "zzz yyy mmm ccc bbb aaa kkk qqq".split()))
    for seed in "1 2 3 4 5 6".split():
        proc = quiet(tool(exe, "--config", config, "--families", path, "--library", f"{START + 16:#x}", "--end", f"{end:#x}", seed=seed))
        yield f"table-first-unknown-key-seed-{seed}", proc, 1, "unknown key `aaa`" 
    yield from bad_table("names-not-a-table", 'names = "x"\n', "`names` is not a table of family names")
    yield from bad_table("bios-not-a-table", 'bios = ["a"]\n', "`bios` is not a table of family names")
    yield from bad_table("folders-not-a-table", "folders = 5\n", "`folders` is not a table of family names")
    yield from bad_table("empty-family", '[bios]\n"a0:05" = "x"\n"b0:05" = ""\n', "`bios` is not a table of family names")
    yield from bad_table("number-family", '[folders]\ngpu = 5\n', "`folders` is not a table of family names")
    yield from bad_table("table-family", '[names]\nn = { a = "b" }\n', "`names` is not a table of family names")
    yield from bad_table("broken", "[names\n", "(at line")
    yield "table-missing", run(table_path=root / "nothing.toml"), 1, "No such file or directory"
    ok = root / "only-names.toml"
    ok.write_text('[names]\nn = "x"\n')
    yield "table-one-part", run(table_path=ok), 0, "swept"

    def bad_config(name: str, text: str, message: str):
        path = root / f"config-{name}.toml"
        path.write_text(text)
        yield f"config-{name}", run(config_path=path), 1, message

    yield from bad_config("broken", "[[unit\n", "(at line")
    yield from bad_config("no-address", '[[unit]]\nsource = "a.c"\nfunctions = [{ name = "f" }]\n', "has no integer address")
    yield from bad_config("string-address", config_text([("a.c", None, [("f", "0x80010000")])]), "has no integer address")
    yield from bad_config("float-address", '[[unit]]\nfunctions = [{ address = 1.5 }]\n', "has no integer address")
    yield "config-missing", run(config_path=root / "nothing.toml"), 1, "No such file or directory"
    yield "config-without-units", run(config_path=root / "empty.toml"), 0, "swept"
    junk = root / "junk.exe"
    junk.write_bytes(b"not an executable" * 200)
    yield "not-an-executable", run(executable_path=junk), 1, "not a PS-X executable"
    short = root / "short.exe"
    short.write_bytes(b"PS-X EXE")
    yield "short-executable", run(executable_path=short), 1, "not a PS-X executable"
    yield "executable-missing", run(executable_path=root / "nothing.exe"), 1, "No such file or directory"

    # Required arguments: each one missing ends the run with the status of a usage error.
    full = [exe, "--config", config, "--families", table, "--library", f"{START + 16:#x}", "--end", f"{end:#x}"]
    for drop, label in ((0, "executable"), (1, "config"), (3, "families"), (5, "library"), (7, "end")):
        args = full[:drop] + full[drop + (1 if drop == 0 else 2) :]
        proc = tool(*args)
        yield f"missing-{label}", verdict(proc.returncode == 2 and "required" in proc.stderr, f"{proc.returncode} {proc.stderr}"), 0, "as required"


def modules_cases(root: Path):
    """`--modules`: overlay modules swept from synthetic archives, each content taken alone."""
    A, B, C = 0xA0, 0xB0, 0xC0
    plain = [OPEN, ONE, RETURN, CLOSE]
    E = lambda direct=(), reached=(), reg=0, away=0, opened=False, resident=False: (set(direct), set(reached), reg, away, opened, resident)  # noqa: E731
    a, b, u, z = "alpha", "beta", UNIDENTIFIED, "zeta"
    fill = lambda prefix, n: [(f"{prefix}{k}", lambda m: plain) for k in range(n)]  # noqa: E731

    # The executable: game functions, then library functions. `sh` and `sh2` are where a module is linked.
    entries = [
        ("rg_open", lambda m: [OPEN, JALR_T9, 0, RETURN, CLOSE]),
        ("rg_fam", lambda m: [OPEN, jal(m["lib_a"]), 0, RETURN, CLOSE]),
        ("rg_chain", lambda m: [OPEN, jal(m["rg_fam"]), 0, RETURN, CLOSE]),
        ("rg_closed", lambda m: plain),
        ("sh", lambda m: [OPEN, jal(m["lib_b"]), 0, RETURN, CLOSE]),
        ("sh2", lambda m: plain),
        # Under the chunk of module K: `sk` at its first word, functions of 7, 4, 4, 4, 4, 4 and 2 words, and `ske` at its end.
        ("sk", lambda m: [OPEN, jal(m["lib_a"]), 0, RETURN, CLOSE]),
        ("kp0", lambda m: [OPEN, ONE, ONE, ONE, ONE, RETURN, CLOSE]),
        *[(f"kp{k}", lambda m: plain) for k in range(1, 6)],
        ("kp6", lambda m: [RETURN, CLOSE]),
        ("ske", lambda m: [OPEN, jal(m["lib_b"]), 0, RETURN, CLOSE]),
        ("lib_a", lambda m: stub(A, 5)),
        ("lib_b", lambda m: stub(B, 5)),
        ("lib_u", lambda m: plain),
        ("lib_z", lambda m: stub(C, 5)),
    ]
    code, ex, ex_size = lay_words(entries)
    assert ex["ske"] == ex["sk"] + 136
    assert ex["sh2"] == ex["sh"] + 20
    end = START + 4 * len(code)
    game = [
        E(reg=1, opened=True), E({a}, {a}), E((), {a}), E(), E({b}, {b}), E(),
        E({a}, {a}), E(), E(), E(), E(), E(), E(), E(), E({b}, {b}),
    ]  # fmt: skip
    game = [(d, r, g, w, o) for d, r, g, w, o, _ in game]
    exe_report = report(START, end, game, [(a, "bios"), (b, "bios"), u, (z, "bios")], [])
    XB, YB, TB, QB = 0x80200000, 0x00210000, 0x80230000, 0x80240000
    slots = [XB, YB, ex["sh"], TB, ex["lib_b"], ex["sk"]]

    def content(archive, slot, base, parts, expect):
        words, book, sizes = lay_words(parts, base)
        assert list(expect) == [name for name, _ in parts]
        return {"archive": archive, "slot": slot, "words": words, "book": book, "sizes": sizes, "expect": expect}

    x_parts = [
        ("x_top", lambda m: [OPEN, jal(m["x_mid"]), 0, RETURN, CLOSE]),
        ("x_mid", lambda m: [OPEN, jal(ex["lib_b"]), 0, jal(m["x_leaf"]), 0, RETURN, CLOSE]),
        ("x_leaf", lambda m: [OPEN, jal(ex["lib_a"]), 0, RETURN, CLOSE]),
        ("x_ping", lambda m: [OPEN, jal(ex["lib_u"]), 0, jal(m["x_pong"]), 0, RETURN, CLOSE]),
        ("x_pong", lambda m: [OPEN, jal(ex["lib_b"]), 0, jal(m["x_ping"]), 0, RETURN, CLOSE]),
        ("x_self", lambda m: [OPEN, jal(m["x_self"]), 0, jal(ex["lib_a"]), 0, RETURN, CLOSE]),
        ("x_game", lambda m: [OPEN, jal(ex["rg_chain"]), 0, RETURN, CLOSE]),
        # two game functions and two library functions in one function: each counted once
        ("x_two", lambda m: [OPEN, jal(ex["rg_chain"]), 0, jal(ex["rg_fam"]), 0, jal(ex["lib_a"]), 0, jal(ex["lib_b"]), 0, RETURN, CLOSE]),
        ("x_gclosed", lambda m: [OPEN, jal(ex["rg_closed"]), 0, RETURN, CLOSE]),
        ("x_via", lambda m: [OPEN, jal(m["x_gopen"]), 0, RETURN, CLOSE]),  # below the open function it calls
        ("x_gopen", lambda m: [OPEN, jal(ex["rg_open"]), 0, RETURN, CLOSE]),
        ("x_regopen", lambda m: [OPEN, JALR_T9, 0, JALR_T9, 0, RETURN, CLOSE]),
        # another module's range, a `j` to the middle of a function of its own chunk, an address of the executable that starts nothing
        ("x_far", lambda m: [OPEN, jal(YB + 16), 0, jump(m["x_top"] + 4), 0, jal(ex["rg_fam"] + 4), 0, RETURN, CLOSE]),
        ("x_open_top", lambda m: [OPEN, jal(m["x_open_mid"]), 0, RETURN, CLOSE]),
        ("x_open_mid", lambda m: [OPEN, jal(m["x_open_src"]), 0, RETURN, CLOSE]),
        ("x_open_src", lambda m: [OPEN, JALR_T9, 0, RETURN, CLOSE]),
        ("x_openfam", lambda m: [OPEN, JALR_T9, 0, jal(ex["lib_a"]), 0, RETURN, CLOSE]),  # open, with a family
    ]
    x_expect = {
        "x_top": E((), {a, b}), "x_mid": E({b}, {a, b}), "x_leaf": E({a}, {a}),
        "x_ping": E({u}, {u, b}), "x_pong": E({b}, {u, b}), "x_self": E({a}, {a}),
        "x_game": E((), {a}, resident=True), "x_two": E({a, b}, {a, b}, resident=True), "x_gclosed": E(resident=True),
        "x_via": E(opened=True), "x_gopen": E(opened=True, resident=True),
        "x_regopen": E(reg=2, opened=True), "x_far": E(away=3, opened=True),
        "x_open_top": E(opened=True), "x_open_mid": E(opened=True), "x_open_src": E(reg=1, opened=True), "x_openfam": E({a}, {a}, 1, 0, True),
    }  # fmt: skip
    X = content("A1.PAC", 0, XB, x_parts, x_expect)
    y_parts = [("y_one", lambda m: [OPEN, jal(XB), 0, RETURN, CLOSE]), *fill("y_f", 7)]
    Y = content("A1.PAC", 1, YB, y_parts, {"y_one": E(away=1, opened=True), **{f"y_f{k}": E() for k in range(7)}})
    x2_parts = [("x2_a", lambda m: [OPEN, jal(ex["lib_u"]), 0, RETURN, CLOSE]), *fill("x2_f", 7)]
    X2 = content("A2.PAC", 0, XB, x2_parts, {"x2_a": E({u}, {u}), **{f"x2_f{k}": E() for k in range(7)}})
    # Linked where the executable has code: it has a start at the first word and one at the sixth that the module does not.
    s_parts = [
        ("s0", lambda m: [OPEN, ONE, ONE, ONE, ONE, RETURN, CLOSE]),  # holds the address of `sh2`
        ("s1", lambda m: [OPEN, jump(ex["sh2"]), 0, RETURN, CLOSE]),  # a call elsewhere: the module has no start there
        ("s2", lambda m: [OPEN, jal(ex["sh"]), 0, RETURN, CLOSE]),  # a call to s0, not to the function of the executable
        *fill("s_f", 5),
    ]
    S = content(
        "A2.PAC", 2, ex["sh"], s_parts,
        {"s0": E(), "s1": E(away=1, opened=True), "s2": E(), **{f"s_f{k}": E() for k in range(5)}},
    )  # fmt: skip
    # Linked over a library function of the executable: r1 calls r0, the module's function, not the library function.
    r_parts = [
        ("r0", lambda m: plain),
        ("r1", lambda m: [OPEN, jal(ex["lib_b"]), 0, RETURN, CLOSE]),
        *fill("r_f", 6),
    ]
    R = content("A2.PAC", 4, ex["lib_b"], r_parts, {"r0": E(), "r1": E(), **{f"r_f{k}": E() for k in range(6)}})
    # Linked over `sk` of the executable with no start of its own at the first word (it is a `nop`): a call to it is a call elsewhere.
    # `ske` of the executable starts at the end of the chunk: a call to it is a call to the executable.
    k_parts = [
        ("k_nop", lambda m: [0]),
        ("k0", lambda m: [jal(ex["sk"]), 0, RETURN, CLOSE]),
        ("k1", lambda m: [OPEN, jal(ex["ske"]), 0, RETURN, CLOSE]),
        *fill("k_f", 6),
    ]
    K = content(
        "A2.PAC", 5, ex["sk"], [(n, f) for n, f in k_parts if n != "k_nop"], {"k0": E(away=1, opened=True), "k1": E((), {b}, resident=True), **{f"k_f{k}": E() for k in range(6)}}
    )  # fmt: skip
    K["words"] = [0, *K["words"]]
    K["book"] = {name: address + 4 for name, address in K["book"].items()}
    assert 4 * len(K["words"]) == 136
    # A function that opens a frame in its middle: a symbol there splits it.
    split = lambda m: [OPEN, jal(0x80500000), 0, ONE, 0x27BDFFE0, ONE, RETURN, CLOSE]  # noqa: E731
    t_fill = {f"t_f{k}": E() for k in range(7)}
    T = content("A1.PAC", 3, TB, [("t0", split), *fill("t_f", 7)], {"t0": E(away=1, opened=True), **t_fill})
    TS = content(
        "A1.PAC", 3, TB,
        [("t_a", lambda m: [OPEN, jal(0x80500000), 0, ONE]), ("t_b", lambda m: [0x27BDFFE0, ONE, RETURN, CLOSE]), *fill("t_f", 7)],
        {"t_a": E(away=1, opened=True), "t_b": E(), **t_fill},
    )  # fmt: skip
    assert TS["words"] == T["words"]
    # Never read: a chunk of table 1 and one in a slot beyond the table, with code that calls a library function.
    q = lambda n: [("q0", lambda m: [OPEN, jal(ex["lib_z"]), 0, RETURN, CLOSE]), *fill("q_f", n)]  # noqa: E731
    Q1, Q2 = content("A3.PAC", 0, QB, q(7), {"q0": E(), **{f"q_f{k}": E() for k in range(7)}}), content("A3.PAC", 9, QB, q(8), {"q0": E(), **{f"q_f{k}": E() for k in range(8)}})

    def body(c) -> bytes:
        return struct.pack(f"<{len(c['words'])}I", *c["words"])

    exe, pointers = make_program(START, code, [slots, [QB]])
    exe_path = root / "modules.exe"
    exe_path.write_bytes(exe)
    config = root / "modules.toml"
    config.write_text("")
    table = root / "modules-families.toml"
    table.write_text('[bios]\n"a0:05" = "alpha"\n"b0:05" = "beta"\n"c0:05" = "zeta"\n')
    good = root / "pac"
    (good / "sub").mkdir(parents=True)
    (good / "A1.PAC").write_bytes(make_archive([(0, body(X)), (1, body(Y)), (3, body(T)), (0, b"data" * 20)]))
    (good / "A2.PAC").write_bytes(make_archive([(0, body(X)), (0, body(X2)), (2, body(S)), (4, body(R)), (5, body(K))]))
    (good / "sub" / "A3.PAC").write_bytes(make_archive([(1 << 16, body(Q1)), (9, body(Q2))]))

    def lines(archives, bad, contents):
        found = [(c, name, exp) for c in contents for name, exp in c["expect"].items()]
        calling = collections.Counter(name for _, _, e in found for name in e[0])
        reaching = collections.Counter(name for _, _, e in found for name in e[1])
        none = [e for _, _, e in found if not e[1]]
        text = [
            f"modules: {archives} archives parsed, {bad} rejected; code-bearing chunks with distinct contents: {len(contents)};"
            f" functions: {len(found)}",
            " family            called directly by  reached by",
        ]
        for name in sorted({a, b, u, z}, key=lambda n: (n == UNIDENTIFIED, n)):
            text.append(f" {name:<17} {calling[name]:>18} {reaching[name]:>11}")
        text += [
            f"module functions that call a library function directly: {sum(1 for _, _, e in found if e[0])}",
            f"module functions that call a game function of the executable: {sum(1 for _, _, e in found if e[5])}",
            f"module functions that reach no library function: {len(none)};"
            f" closed: {sum(1 for e in none if not e[4])}, open: {sum(1 for e in none if e[4])}",
            f"module functions with a call through a register: {sum(1 for _, _, e in found if e[2])};"
            f" with a call elsewhere: {sum(1 for _, _, e in found if e[3])}",
        ]
        rows = "".join(
            f"{c['archive']}\t{c['slot']:#x}\t{c['book'][name]:08x}\t{c['sizes'][name]}\t{joined(e[0])}\t{joined(e[1])}"
            f"\t{e[2]}\t{e[3]}\t{'open' if e[4] else 'closed'}\n"
            for c, name, e in found
        )
        return "".join(line + "\n" for line in text), rows

    base = [exe_path, "--config", config, "--families", table, "--library", f"{ex['lib_a']:#x}", "--end", f"{end:#x}"]
    with_modules = [*base, "--modules", good, "--pointers", f"{pointers:#x}"]
    contents = [X, Y, T, X2, S, R, K]
    text, rows = lines(3, 0, contents)
    out_exe, out_mod = root / "m" / "game.tsv", root / "mm" / "deeper" / "modules.tsv"
    plain_out = root / "m" / "plain.tsv"
    proc = tool(*with_modules, "--out", out_exe, "--modules-out", out_mod)
    yield "modules-exact", exact(proc, exe_report + text), 0, "as required"
    yield "modules-no-out-file", exact(tool(*with_modules), exe_report + text), 0, "as required"
    got = out_mod.read_text() if out_mod.exists() else None
    yield "modules-out-rows", verdict(got == rows, f"{got!r}\n{rows!r}"), 0, "as required"
    bare = tool(*base, "--out", plain_out)
    same = out_exe.exists() and plain_out.exists() and out_exe.read_text() == plain_out.read_text()
    yield "without-modules-exact", exact(bare, exe_report), 0, "as required"
    yield "modules-leave-game-rows", verdict(same and "modules" not in bare.stdout, "game rows differ"), 0, "as required"
    # A symbol at the middle of the function splits it in two.
    symbols = root / "symbols.txt"
    symbols.write_text(f"t_b = {TB + 16:#x};\n")
    split_text, split_rows = lines(3, 0, [X, Y, TS, X2, S, R, K])
    out_sym = root / "m" / "symbols.tsv"
    proc = tool(*with_modules, "--symbols", symbols, "--modules-out", out_sym)
    yield "modules-symbols", exact(proc, exe_report + split_text), 0, "as required"
    got = out_sym.read_text() if out_sym.exists() else None
    yield "modules-symbols-rows", verdict(got == split_rows, f"{got!r}\n{split_rows!r}"), 0, "as required"
    # An address that is no multiple of four is not a symbol.
    odd_symbol = root / "odd-symbols.txt"
    odd_symbol.write_text(f"{TB + 18:#x}\n")
    yield "modules-unaligned-symbol", exact(tool(*with_modules, "--symbols", odd_symbol), exe_report + text), 0, "as required"
    # A line that is a bare address is an entry of the sweep and carries no name: the same split, with a name beside it.
    bare = root / "bare-symbols.txt"
    bare.write_text(f"{TB + 16:#x}\nsome_name = {ex['lib_a']:#x};\n")
    out_bare = root / "m" / "bare.tsv"
    proc = tool(*with_modules, "--symbols", bare, "--modules-out", out_bare)
    yield "modules-bare-address-entry", exact(proc, exe_report + split_text), 0, "as required"
    got = out_bare.read_text() if out_bare.exists() else None
    yield "modules-bare-address-rows", verdict(got == split_rows, f"{got!r}\n{split_rows!r}"), 0, "as required"
    # An archive that is rejected is reported, and the rest is still printed.
    bad = root / "pac-bad"
    bad.mkdir()
    (bad / "A1.PAC").write_bytes((good / "A1.PAC").read_bytes())
    (bad / "Z.PAC").write_bytes(b"short")
    bad_text, bad_rows = lines(1, 1, [X, Y, T])
    out_bad = root / "m" / "bad.tsv"
    proc = tool(*base, "--modules", bad, "--pointers", f"{pointers:#x}", "--modules-out", out_bad)
    want = exe_report + "Z.PAC: shorter than one sector\n" + bad_text
    yield "modules-rejected-archive", verdict(proc.returncode == 1 and proc.stdout == want, f"{proc.returncode}\n{proc.stdout}\n{want}"), 0, "as required"
    got = out_bad.read_text() if out_bad.exists() else None
    yield "modules-rejected-rows", verdict(got == bad_rows, f"{got!r}"), 0, "as required"

    def refused(name: str, extra, message: str):
        proc = quiet(tool(*extra))
        yield name, verdict(proc.returncode == 1 and proc.stdout == exe_report + message + "\n", f"{proc.returncode}\n{proc.stdout}{proc.stderr}"), 0, "as required"

    yield from refused("modules-no-pointer-block", [*base, "--modules", good, "--pointers", f"{START:#x}"], f"no block of table addresses at {START:#x}")
    data_only = root / "pac-data"
    data_only.mkdir()
    (data_only / "D.PAC").write_bytes(make_archive([(0, b"data" * 20)]))
    yield from refused("modules-no-code-chunk", [*base, "--modules", data_only, "--pointers", f"{pointers:#x}"], "no code-bearing chunk found")
    empty = root / "pac-empty"
    empty.mkdir()
    yield from refused("modules-no-archive", [*base, "--modules", empty, "--pointers", f"{pointers:#x}"], "no code-bearing chunk found")
    odd, odd_pointers = make_program(START, code, [[XB + 2, YB, ex["sh"], TB], [QB]])
    odd_path = root / "odd.exe"
    odd_path.write_bytes(odd)
    odd_args = [odd_path, *base[1:], "--modules", good, "--pointers", f"{odd_pointers:#x}"]
    yield from refused("modules-unaligned-destination", odd_args, f"slot 0x0: the destination {XB + 2:#x} is not a multiple of four")
    proc = tool(*base, "--modules", good)
    yield "modules-need-pointers", verdict(proc.returncode == 2 and "--modules needs --pointers" in proc.stderr, proc.stderr), 0, "as required"


def names_cases(root: Path):
    """`--symbols`: other names of library functions, from the `name = value;` statements of the symbol file."""
    plain = [OPEN, ONE, RETURN, CLOSE]
    entries = [
        ("l_unk", lambda a: plain),  # declared under a name the table lacks
        ("l_und", lambda a: plain),  # declared by no unit
        ("l_both", lambda a: plain),  # declared under a name the table knows
        ("l_order", lambda a: plain),  # three other names
        ("l_case", lambda a: plain),  # two other names that differ in case
        ("l_bios", lambda a: stub(0xA0, 5)),
        ("l_folder", lambda a: plain),
        ("l_cold", lambda a: stub(0xA0, 5)),  # an other name that the table lacks: the call decides
        ("l_fold2", lambda a: plain),  # the same, for the folder
        ("l_none", lambda a: plain),
        ("l_oct", lambda a: plain),
        ("l_dec", lambda a: plain),
        ("l_hex", lambda a: plain),
        ("l_comment", lambda a: plain),
        ("l_bare", lambda a: plain),
        ("l_tight", lambda a: plain),
        ("l_lead", lambda a: plain),  # a name that starts with an underscore
        ("l_digit", lambda a: plain),  # a name with a digit inside
        ("l_unterm", lambda a: plain),  # a statement without its semicolon
    ]
    exe_bytes, a, size = lay_out(entries)
    end = START + len(exe_bytes) - 0x800
    exe = root / "names.exe"
    exe.write_bytes(exe_bytes)
    config = root / "names.toml"
    config.write_text(
        config_text(
            [
                ("sdk/gpu/a.c", None, [("unk_name", a["l_unk"]), ("zz_known", a["l_both"]), (None, a["l_folder"]), (None, a["l_fold2"])]),
                ("sdk/gpu/b.c", None, [(None, a["l_bios"]), (None, a["l_cold"])]),
            ]
        )
    )
    table = root / "names-families.toml"
    table.write_text(
        '[names]\nzz_known = "famC"\naa_other = "famOther"\nalias_a = "famA"\nalias_b = "famB"\nm_name = "famM"\nb_name = "famB2"\n'
        'Zed = "famZ"\nalpha_n = "famAl"\nali_bios = "famX"\nali_folder = "famF"\noct_name = "famO"\ndec_name = "famD"\n'
        'hex_name = "famH"\ncm_name = "famCm"\ntight = "famT"\nnever = "famNever"\n_lead = "famLead"\nn9_x = "famNine"\nunterm = "famUnterm"\n\n[bios]\n"a0:05" = "alpha"\n\n[folders]\ngpu = "delta"\n'
    )
    octal = f"0{a['l_oct']:o}"
    symbols = root / "names-symbols.ld"
    symbols.write_text(
        "/* a symbol file */\n"
        f"alias_a = {a['l_unk']:#x};\n"
        f"alias_b = {a['l_und']:#x};\n"
        f"aa_other = {a['l_both']:#x};\n"
        f"zz_known = {a['l_both']:#x};\n"
        f"m_name = {a['l_order']:#x};\n"
        f"a_name = {a['l_order']:#x};\n"
        f"b_name = {a['l_order']:#x};\n"
        f"alpha_n = {a['l_case']:#x};\n"
        f"Zed = {a['l_case']:#x};\n"
        f"ali_bios = {a['l_bios']:#x}; ali_folder = {a['l_folder']:#x};\n"
        f"nope = {a['l_cold']:#x};\nnope2 = {a['l_fold2']:#x};\n"
        f"oct_name = {octal};\ndec_name = {a['l_dec']};\nhex_name = 0X{a['l_hex']:X};\n"
        f"/* a note\n   cm_name = {a['l_comment']:#x};\n   more */\n"
        f"{a['l_bare']:#x}\n"
        f"never = {a['l_bare'] + 4:#x};\n"
        f"tight={a['l_tight']:#x};\n"
        f"_lead = {a['l_lead']:#x};\nn9_x = {a['l_digit']:#x};\n"
        f"unterm = {a['l_unterm']:#x}\n"
    )
    family = {
        "l_unk": "famA", "l_und": "famB", "l_both": "famC", "l_order": "famB2", "l_case": "famZ", "l_bios": "famX",
        "l_folder": "famF", "l_cold": "alpha", "l_fold2": "delta", "l_none": UNIDENTIFIED, "l_oct": "famO", "l_dec": "famD",
        "l_hex": "famH", "l_comment": UNIDENTIFIED, "l_bare": UNIDENTIFIED, "l_tight": "famT",
        "l_lead": "famLead", "l_digit": "famNine", "l_unterm": UNIDENTIFIED,
    }  # fmt: skip
    declared = {"l_unk": "unk_name", "l_both": "zz_known"}
    rule = dict.fromkeys(family, "name")
    rule.update({"l_cold": "bios", "l_fold2": "folder", "l_none": "-", "l_comment": "-", "l_bare": "-", "l_unterm": "-"})
    base = [exe, "--config", config, "--families", table, "--library", f"{START:#x}", "--end", f"{end:#x}"]

    def rows(families: dict, rules: dict) -> str:
        return "".join(f"{a[n]:08x}\t{size[n]}\t{declared.get(n, '-')}\t{families[n]}\t0\t{rules[n]}\n" for n, _ in entries)

    out = root / "names-lib.tsv"
    proc = tool(*base, "--symbols", symbols, "--library-out", out)
    got = out.read_text() if out.exists() else None
    want = rows(family, rule)
    yield "names-library-rows", verdict(proc.returncode == 0 and got == want, f"{proc.stdout}{proc.stderr}{got!r}\n{want!r}"), 0, "as required"
    # Without the symbol file a function has its declared name, its call or its folder, as before.
    plain_family = dict.fromkeys(family, UNIDENTIFIED)
    plain_family.update({"l_both": "famC", "l_bios": "alpha", "l_cold": "alpha", "l_folder": "delta", "l_fold2": "delta", "l_unk": "delta", "l_und": "delta"})
    plain_rule = dict.fromkeys(family, "-")
    plain_rule.update({"l_both": "name", "l_bios": "bios", "l_cold": "bios", "l_folder": "folder", "l_fold2": "folder", "l_unk": "folder", "l_und": "place"})
    out2 = root / "names-lib-plain.tsv"
    proc = tool(*base, "--library-out", out2)
    got = out2.read_text() if out2.exists() else None
    want = rows(plain_family, plain_rule)
    yield "names-without-symbols", verdict(proc.returncode == 0 and got == want, f"{proc.stdout}{proc.stderr}{got!r}\n{want!r}"), 0, "as required"
    # The family table counts what the second names decide.
    counts = collections.Counter(family.values())
    text = tool(*base, "--symbols", symbols).stdout
    wanted = [f" {n:<17} {counts[n]:>17} {0:>19} {0:>11}\n" for n in sorted(counts, key=lambda n: (n == UNIDENTIFIED, n))]
    yield "names-family-lines", verdict(all(line in text for line in wanted), text), 0, "as required"

    # A value that the linker would not read as an integer ends the run, with the message and without a traceback.
    for label, value in (
        ("octal-digit", "09"), ("octal-eight", "0128"), ("hex-letters", "0xZZ"), ("hex-empty", "0x"), ("a-word", "bar"), ("a-fraction", "1.5"),
    ):  # fmt: skip
        bad = root / f"names-bad-{label}.ld"
        bad.write_text(f"alias_a = 0x80010000;\nbroken = {value};\n")
        proc = quiet(tool(*base, "--symbols", bad))
        yield f"names-bad-value-{label}", verdict(
            proc.returncode == 1 and f"{bad}: `broken` is assigned `{value}`, which is no integer as the linker reads it" in proc.stdout and proc.stdout.count("\n") == 1,
            f"{proc.returncode}\n{proc.stdout}{proc.stderr}",
        ), 0, "as required"
    # Zero is an integer, in each form.
    for label, value in (("zero", "0"), ("hex-zero", "0x0"), ("octal-zero", "00")):
        zero = root / f"names-zero-{label}.ld"
        zero.write_text(f"zero_name = {value};\n")
        yield f"names-value-{label}", quiet(tool(*base, "--symbols", zero)), 0, "swept"
    missing = quiet(tool(*base, "--symbols", root / "nothing.ld"))
    yield "names-symbols-missing", missing, 1, "No such file or directory"
    # Statements are read from comments out only: a value after a comment, and a comment after a statement.
    mixed = root / "names-mixed.ld"
    mixed.write_text(f"/* x */ alias_a = {a['l_unk']:#x}; /* y */\n")
    out3 = root / "names-lib-mixed.tsv"
    proc = tool(*base, "--symbols", mixed, "--library-out", out3)
    got = out3.read_text() if out3.exists() else None
    want = rows({**plain_family, "l_unk": "famA"}, {**plain_rule, "l_unk": "name"})
    yield "names-comment-around-statement", verdict(proc.returncode == 0 and got == want, f"{got!r}\n{want!r}"), 0, "as required"
    # A name starts with a letter or an underscore: `9alias_a` is no name, and its tail `alias_a` is not taken for one.
    digit = root / "names-digit-first.ld"
    digit.write_text(f"9alias_a = {a['l_unk']:#x};\n")
    out4 = root / "names-lib-digit.tsv"
    proc = tool(*base, "--symbols", digit, "--library-out", out4)
    got = out4.read_text() if out4.exists() else None
    want = rows(plain_family, plain_rule)
    yield "names-digit-first-is-no-name", verdict(proc.returncode == 0 and got == want, f"{got!r}\n{want!r}"), 0, "as required"


def place_cases(root: Path):
    """The family of a library function by its place: between two declared functions of one reference file."""
    plain = [OPEN, ONE, RETURN, CLOSE]
    D, U = "delta", UNIDENTIFIED
    # (name, source of the unit that declares it or None, declared name or None, body, family, rule)
    lib = [
        ("q0", None, None, plain, U, "-"),  # no anchor below
        ("p0", "sdk/gpu/sys_p1.c", None, plain, D, "folder"),
        ("p1", None, None, plain, D, "place"),  # undeclared, between two parts of one file
        ("p2", None, None, plain, D, "place"),  # as many as lie between
        ("p3", "sdk/gpu/sys_p12.c", None, plain, D, "folder"),
        ("p4", "other/gpu/sys_p2.c", None, plain, D, "place"),  # declared outside sdk/: no anchor
        ("p5", "sdk/gpu/sys_p2a.c", None, plain, D, "folder"),
        ("p6", None, None, stub(0xA0, 5), "alpha", "bios"),  # keeps its family
        ("p7", "sdk/gpu/sys.s", None, plain, D, "folder"),
        ("p8", "other/x.c", "n_known", plain, "famN", "name"),  # keeps its family
        ("p9", "sdk/gpu/sys_p3.c", None, plain, D, "folder"),
        ("p10", None, None, plain, U, "-"),  # `sys` and `sysp1` are two files
        ("p11", "sdk/gpu/sysp1.c", None, plain, D, "folder"),
        ("p12", None, None, plain, U, "-"),  # `sysp1` and `sys_px`
        ("p13", "sdk/gpu/sys_px.c", None, plain, D, "folder"),
        ("p14", None, None, plain, U, "-"),  # `sys_px` and `sys`
        ("p15", "sdk/gpu/sys_p9.c", None, plain, D, "folder"),
        ("r0", "sdk/gpu/sub/x.c", None, plain, D, "folder"),
        ("r1", None, None, plain, D, "place"),  # a deeper path keeps its folders, the family is that of `gpu`
        ("r2", "sdk/gpu/sub/x_p2.c", None, plain, D, "folder"),
        ("r3", None, None, plain, U, "-"),  # `gpu/sub/x` and `gpu/x`
        ("r4", "sdk/gpu/x.c", None, plain, D, "folder"),
        ("s0", "sdk/spu/e.c", None, plain, U, "-"),  # a folder that the table lacks
        ("s1", None, None, plain, U, "-"),
        ("s2", "sdk/spu/e_p1.c", None, plain, U, "-"),
        ("t0", "sdk/gpu/ff.c", None, plain, D, "folder"),
        ("t1", None, None, plain, U, "-"),  # an anchor of another file lies between two of `gpu/ff`
        ("t2", "sdk/gpu/gg.c", None, plain, D, "folder"),
        ("t3", None, None, plain, U, "-"),
        ("t4", "sdk/gpu/ff.c", None, plain, D, "folder"),
        ("u0", "sdk/gpu/hh.c", None, plain, D, "folder"),
        ("u1", None, None, plain, U, "-"),  # a file directly in sdk/ is an anchor of its own, here between two of `gpu/hh`
        ("u2", "sdk/lone.c", None, plain, U, "-"),  # it has no folder and so no family
        ("u3", None, None, plain, U, "-"),
        ("u4", "sdk/gpu/hh.c", None, plain, D, "folder"),
        ("v0", "sdk/gpu.c", None, plain, U, "-"),  # two anchors of one file directly in sdk/: its name is no folder
        ("v1", None, None, plain, U, "-"),
        ("v2", "sdk/gpu_p1.c", None, plain, U, "-"),
        ("z", None, None, plain, U, "-"),  # no anchor above
    ]
    entries = [
        ("g_call", lambda a: [OPEN, jal(a["p1"]), 0, jal(a["p6"]), 0, jal(a["p10"]), 0, RETURN, CLOSE]),
        ("g_anchor", lambda a: plain),  # declared from sdk/gpu/sys_p0.c, and below the library: no anchor
        *[(name, lambda a, body=body: body) for name, _, _, body, *_ in lib],
    ]
    exe_bytes, a, size = lay_out(entries)
    end = START + len(exe_bytes) - 0x800
    exe = root / "place.exe"
    exe.write_bytes(exe_bytes)
    units: dict[str, list] = {"sdk/gpu/sys_p0.c": [(None, a["g_anchor"])]}
    for name, source, declared_name, *_ in lib:
        if source:
            units.setdefault(source, []).append((declared_name, a[name]))
    config = root / "place.toml"
    config.write_text(config_text([(source, None, functions) for source, functions in units.items()]))
    table = root / "place-families.toml"
    table.write_text('[names]\nn_known = "famN"\n\n[bios]\n"a0:05" = "alpha"\n\n[folders]\ngpu = "delta"\n')
    base = [exe, "--config", config, "--families", table, "--library", f"{a['q0']:#x}", "--end", f"{end:#x}"]
    libs = [(family, rule) for *_, family, rule in lib]
    games = [({D, "alpha", U}, {D, "alpha", U}, 0, 0, False), (set(), set(), 0, 0, False)]
    out = root / "place-lib.tsv"
    proc = tool(*base, "--library-out", out)
    yield "place-summary-exact", exact(proc, report(START, end, games, libs, [])), 0, "as required"
    rules = collections.Counter(rule for _, rule in libs)
    assert (rules["name"], rules["bios"], rules["folder"], rules["place"]) == (1, 1, 16, 4)
    callers = {"p1": 1, "p6": 1, "p10": 1}
    want = "".join(
        f"{a[name]:08x}\t{size[name]}\t{declared_name or '-'}\t{family}\t{callers.get(name, 0)}\t{rule}\n"
        for name, _, declared_name, _, family, rule in lib
    )
    got = out.read_text() if out.exists() else None
    yield "place-library-rows", verdict(got == want, f"{got!r}\n{want!r}"), 0, "as required"
    # Without anchors of the table's folders there is no place: the same layout with no folders.
    bare = root / "place-no-folders.toml"
    bare.write_text('[names]\nn_known = "famN"\n\n[bios]\n"a0:05" = "alpha"\n')
    off = [(family if rule in ("name", "bios") else U, rule if rule in ("name", "bios") else "-") for family, rule in libs]
    proc = tool(exe, "--config", config, "--families", bare, "--library", f"{a['q0']:#x}", "--end", f"{end:#x}")
    yield "place-needs-the-folder", exact(proc, report(START, end, [({"alpha", U}, {"alpha", U}, 0, 0, False), (set(), set(), 0, 0, False)], off, [])), 0, "as required"
    # A table without `[folders]` at all.
    none = root / "place-no-table.toml"
    none.write_text("")
    proc = tool(exe, "--config", config, "--families", none, "--library", f"{a['q0']:#x}", "--end", f"{end:#x}")
    yield "place-empty-table", exact(proc, report(START, end, [({U}, {U}, 0, 0, False), (set(), set(), 0, 0, False)], [(U, "-")] * len(lib), [])), 0, "as required"


def cases(root: Path):
    yield from main_cases(root)
    yield from mini_cases(root)
    yield from table_cases(root)
    yield from modules_cases(root)
    yield from names_cases(root)
    yield from place_cases(root)


def main() -> int:
    failed = 0
    with tempfile.TemporaryDirectory() as tmp:
        for name, proc, want_status, want_text in cases(Path(tmp)):
            output = proc.stdout + proc.stderr
            ok = proc.returncode == want_status and want_text in output
            print(f"{'ok  ' if ok else 'FAIL'} {name}: exit {proc.returncode}, wanted {want_status} with {want_text!r}")
            if not ok:
                print(output)
                failed += 1
    print(f"{failed} case(s) behaved wrongly" if failed else "all cases behaved as required")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
