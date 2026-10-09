# Requirements and completion evidence

Parent: [delivery plan](../PLAN.md). These are requirements, not claims that a
full build or decompilation already exists.

## Inputs and identity

- Use user-supplied inputs and pin revision, size, hashes, file provenance, and
  extraction mapping. Do not alter original disc images or installations.
- Include the resident executable and any executable overlays needed for the
  chosen game revision. Treat file extensions as hints, not evidence of content.
- Preserve original asset dependencies externally. Do not mistake a compiler
  library or modern wrapper for the gameplay program.
- Distinguish compiler-compatible output from a uniquely proven historical
  compiler/SDK version. Record tools, flags, hashes, link addresses and fixups.

## Source and ownership

- Keep generated analysis, inferred meanings, reviewed source, reference bytes,
  and test status separate. New analysis must not overwrite refinements.
- Each exact output range needs a declared owner. Reject unintended gaps,
  overlaps, bad addresses, silent truncation, or fallback substitution.
- Prefer readable C with explicit widths/layouts and meaningful domain types.
  Unknown fields stay unknown; inferred names are not recovered symbols.
- Allow documented assembly exceptions and separately validated nonmatching C.
  Do not count all-assembly gameplay as satisfying readable gameplay C.
- Count matching C, assembly, SDK code, data, and raw retention separately.
  Opaque executable retention is useful scaffolding, not completed source work.
- A port, if one is made: by the owner's direction of 2026-10-06 the game
  code stays as close to the original as possible and only Sony's library
  code is swapped. Differences belong in the library replacement and a thin
  platform layer. What game code cannot keep on another machine, such as
  pointer size, fixed addresses and the loading of modules, goes behind
  build switches and is listed. The owner decided on 2026-10-06 to start
  one, for macOS on Apple Silicon, Windows and Linux: its decisions and
  state are on the [port page](../port/README.md). Nothing of the game
  links or runs there yet. By the owner's rule of 2026-10-09, until the
  port is shown running nothing of the game's source is changed for it,
  build switches included, and the port takes its data from a disc image
  that the user supplies.

## Validation

- Compare complete claimed bytes after actual compilation/linking, at the
  correct addresses with all required references resolved. No wildcard matches.
- Fail checks on stale inputs, wrong tool versions, errors, missing outputs, or
  false success. Keep negative controls for the comparison/build pipeline.
- Exact bytes and mapping establish instruction identity. No additional boot,
  runtime injection, replay or rebuilt-disc test is mandatory for this property.
- Optional runtime research must identify the question and exact artifact it
  observes. A routine's matching status does not prove its guessed meaning.
- Nonmatching candidates need explicit ABI/input/output/alias contracts and
  scoped differential tests. Test passing is not universal equivalence.
- Reproduce final authoritative outputs from clean build products and retained
  source/tool manifests. Document all missing coverage and assembly exceptions.

## Distribution and operations

- No game binaries, extracted assets, analysis databases, credentials,
  toolchain binaries or third-party files under another license enter Git.
  All source that the project writes is published, by the owner's decisions
  of 2026-10-05 (the PS1 source in `ps1/src/`, after a provenance review)
  and 2026-10-06 (all of it, the source of overlay modules and the Windows
  reconstruction included). Source that enters Git for the first time gets
  the same review first. Decompiler output and other artifacts derived from
  the game are not source the project writes and stay private, with one
  exception by the owner's decision of 2026-10-06: the function inventory
  of the static sweep, `ps1/inventory/`, addresses and sizes without bytes,
  from which the repository's workflow draws the coverage map.
- Private workspaces are ignored by directory, not only extension. Suffixless
  executables and `.c` pseudocode must remain protected too.
- Review external implementation licenses/provenance before copying. Referencing
  a tool's build metadata is not permission to copy a game's implementation.
- Use scoped environments and preserve unrelated changes. The old 68k repository
  is not a runtime dependency of the migrated project.
- Preserve original historical reports; record migration adjustments and new
  validation separately. Do not rewrite prior measurements as current evidence.
