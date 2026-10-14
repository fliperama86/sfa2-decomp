## PS1 retained-intro-code probe (2026-10-14)

The owner corrected the proposed use of a recording as a timing reference:
recover the intro from code, not visual mimicry. CPS2 is the arcade authority;
Saturn decompilation is a guide. The owner also proposed that the PS1 disc
may retain the original in-engine controller. Investigate that before writing
replacement scene logic. Nothing under `ps1/` changes for this feature.

### Static findings

- The resident program-index table resolves the normal opening request from
  the attract module to `PAC/Z2O.EXE`, the existing movie player. The alternate
  request selects `PAC/BF3.EXE`. This follows original tables and existing C,
  not an inference from filenames alone.
- The supplied disc's `A2O.EXE` and `BF3.EXE` differ from `Z2O.EXE` by only
  seven and eight bytes respectively, confined to four instructions in main.
  The other 26 declared game functions are byte-identical. These executables
  are variants of that player, not separate sprite-intro implementations.
- The portrait texture block previously verified in `END00.PAC` is identical
  in all 21 extracted ending archives. Its loader destination matches the
  texture pointer installed by `spawn_objects_by_state_index_slot28`.
- The resident object dispatch table connects that spawner's portrait object
  type to `call_handler_by_state_0_21_slot28`. Its initializer selects two
  sprite sequences through `init_object_from_position_table_by_index_slot28`.
  Decoding their referenced layouts finds all 211 previously verified
  nonuniform portrait cells. This establishes existing PS1 code/data references
  to the portrait, not just the presence of loose pixels.
- That path belongs to the ending module's scrolling scene. It does not
  establish the arcade opening's flame animation, flashes, timing or sound.

The source and mapping used here are from main `e7de59495acafa9943471aa5b2085cfa94bda607`.
Program inputs were read from the supplied disc and checked against the private
baseline hashes. Existing exact-C evidence was reused; no PS1 build, port build,
game launch or unchanged asset-decoder test was repeated.

### Limits and follow-up

No complete retained PS1 arcade-intro controller has been identified. The
checked opening routes and sprite consumers do not establish absence elsewhere
on the disc. A literal timing-constant search is only a lead: table-driven or
retimed code could evade it. Compare controller behavior and referenced data,
not function names or cross-architecture instruction bytes.

Private evidence remains in `port/local/intro-asset-probe/`: the
`ps1-code-route-probe`, `ps1-program-routes`, `ps1-portrait-consumers`,
`ps1-portrait-aliases` and `ps1-portrait-handler-route` JSON reports. They contain
extracted values and stay ignored. The earlier background-reference evidence
supplies the indexed-pixel identities; palette equivalence remains unproven.
This increment records findings and the corrected recovery method only. No
scene player, experimental setting or change to default runtime behavior exists.
