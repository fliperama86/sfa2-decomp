#!/usr/bin/env python3
"""Pin and verify a baseline manifest for a raw 2352-byte-sector PS1 disc image.

`pin` reads an audit inventory plus the disc image and writes a manifest with
the image size/SHA-256 and, per file, path, LBA, size, SHA-256 and PS-X EXE
header fields. File content is the first `size` bytes of the concatenated
2048-byte user-data fields starting at the file's LBA. `verify` recomputes and
compares against the image, a directory of extracted files, or both.
"""

import argparse
import hashlib
import json
import struct
import sys
from pathlib import Path

USER_DATA = 2048
EXE_MAGIC = b"PS-X EXE"
CHUNK = 1 << 20


def hash_image(path):
    """Return (size, sha256 hex) of the whole image, streamed."""
    h = hashlib.sha256()
    size = 0
    with open(path, "rb") as f:
        while block := f.read(CHUNK):
            h.update(block)
            size += len(block)
    return size, h.hexdigest()


def read_file_bytes(img, lba, size, sector_size, payload_offset):
    """Yield the file content in pieces, one user-data field per sector."""
    remaining = size
    sector = lba
    while remaining > 0:
        img.seek(sector * sector_size + payload_offset)
        piece = img.read(min(USER_DATA, remaining))
        if len(piece) != min(USER_DATA, remaining):
            raise ValueError(f"short read at sector {sector}")
        yield piece
        remaining -= len(piece)
        sector += 1


def hash_disc_file(img, lba, size, sector_size, payload_offset):
    """Return (sha256 hex, first 2048 bytes) of a disc file."""
    h = hashlib.sha256()
    head = b""
    for piece in read_file_bytes(img, lba, size, sector_size, payload_offset):
        if len(head) < USER_DATA:
            head += piece
        h.update(piece)
    return h.hexdigest(), head[:USER_DATA]


def exe_header(head):
    """Header fields if `head` starts with the PS-X EXE magic, else None."""
    if not head.startswith(EXE_MAGIC) or len(head) < 0x34:
        return None
    pc0, = struct.unpack_from("<I", head, 0x10)
    t_addr, t_size = struct.unpack_from("<II", head, 0x18)
    stack, = struct.unpack_from("<I", head, 0x30)
    return {"pc0": pc0, "t_addr": t_addr, "t_size": t_size,
            "stack": stack, "header_size": USER_DATA}


def cmd_pin(args):
    inv = json.loads(Path(args.inventory).read_text())
    image = args.image or inv["input"]
    size, digest = hash_image(image)
    if size != inv["size"] or digest != inv["sha256"]:
        print(f"image mismatch: size {size} sha256 {digest}", file=sys.stderr)
        return 1
    ss, po = inv["sector_size"], inv["payload_offset"]
    files = []
    with open(image, "rb") as img:
        for ent in sorted(inv["files"], key=lambda e: e["path"]):
            sha, head = hash_disc_file(img, ent["lba"], ent["size"], ss, po)
            rec = {"path": ent["path"], "lba": ent["lba"],
                   "size": ent["size"], "sha256": sha}
            hdr = exe_header(head)
            if hdr:
                rec["exe"] = hdr
            files.append(rec)
    manifest = {"image": {"size": size, "sha256": digest},
                "sector_size": ss, "payload_offset": po, "files": files}
    out = Path(args.out)
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps(manifest, indent=2) + "\n")
    print(f"pinned {len(files)} files, image sha256 {digest}")
    return 0


def sha256_file(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        while block := f.read(CHUNK):
            h.update(block)
    return h.hexdigest(), path.stat().st_size


def cmd_verify(args):
    if not args.image and not args.files:
        print("verify needs --image and/or --files", file=sys.stderr)
        return 2
    man = json.loads(Path(args.manifest).read_text())
    ok = True
    if args.image:
        size, digest = hash_image(args.image)
        if size != man["image"]["size"] or digest != man["image"]["sha256"]:
            print(f"IMAGE MISMATCH: size {size} sha256 {digest}")
            ok = False
        else:
            print("image hash matches")
        matched = bad = 0
        ss, po = man["sector_size"], man["payload_offset"]
        with open(args.image, "rb") as img:
            for ent in man["files"]:
                sha, _ = hash_disc_file(img, ent["lba"], ent["size"], ss, po)
                if sha == ent["sha256"]:
                    matched += 1
                else:
                    bad += 1
                    ok = False
                    print(f"MISMATCH (image): {ent['path']}")
        print(f"image files: {len(man['files'])} total, {matched} matched, "
              f"{bad} mismatched")
    if args.files:
        root = Path(args.files)
        present = matched = bad = absent = 0
        for ent in man["files"]:
            p = root / ent["path"]
            if not p.is_file():
                absent += 1
                continue
            present += 1
            sha, size = sha256_file(p)
            if sha == ent["sha256"] and size == ent["size"]:
                matched += 1
            else:
                bad += 1
                ok = False
                print(f"MISMATCH (files): {ent['path']}")
        print(f"files: {present} present, {matched} matched, {bad} mismatched, "
              f"{absent} absent")
        if matched == 0:
            print("no manifest paths matched under --files")
            ok = False
    return 0 if ok else 1


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("pin", help="write a manifest from an inventory and image")
    p.add_argument("--inventory", required=True)
    p.add_argument("--out", required=True)
    p.add_argument("--image")
    p.set_defaults(fn=cmd_pin)
    v = sub.add_parser("verify", help="compare a manifest to an image or files")
    v.add_argument("--manifest", required=True)
    v.add_argument("--image")
    v.add_argument("--files")
    v.set_defaults(fn=cmd_verify)
    args = ap.parse_args()
    sys.exit(args.fn(args))


if __name__ == "__main__":
    main()
