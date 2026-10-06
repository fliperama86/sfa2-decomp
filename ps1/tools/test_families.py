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


def lay_out(entries: list, start: int = START) -> tuple[bytes, dict[str, int], dict[str, int]]:
    """Place functions one after another. Each entry is (name, body), the body a function of the address book.

    Returns the executable, the address of each function and its size in bytes.
    """
    sizes = {name: 4 * len(body(collections.defaultdict(int))) for name, body in entries}
    book, cursor = {}, start
    for name, _ in entries:
        book[name], cursor = cursor, cursor + sizes[name]
    words = [word for _, body in entries for word in body(book)]
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
    libs: the family of each library function. outside: declared addresses that are no start.
    """
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
    libs = [family[n] for n in lnames]
    out, lib_out = root / "deep" / "er" / "game.tsv", root / "other" / "deeper" / "library.tsv"
    proc = tool(*base, "--out", out, "--library-out", lib_out)
    yield "summary-exact", exact(proc, report(START, end, rows, libs, outside)), 0, "as required"
    want_out = "".join(
        f"{a[n]:08x}\t{size[n]}\t{named_game.get(n, '-')}\t{joined(d)}\t{joined(r)}\t{reg}\t{other}\t{'open' if opened else 'closed'}\n"
        for n, (d, r, reg, other, opened) in game.items()
    )
    got = out.read_text() if out.exists() else None
    yield "out-rows", verdict(got == want_out, f"{got!r}\n{want_out!r}"), 0, "as required"
    want_lib = "".join(f"{a[n]:08x}\t{size[n]}\t{named_library.get(n, '-')}\t{family[n]}\t{callers.get(n, 0)}\n" for n in lnames)
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
        yield f"stub-at-end-{label}", exact(proc, report(START, end, [(direct, direct, 0, 0, False)], [family], [])), 0, "as required"

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


def cases(root: Path):
    yield from main_cases(root)
    yield from mini_cases(root)
    yield from table_cases(root)


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
