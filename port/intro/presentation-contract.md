# Native intro presentation

`presentation.c` reconstructs the intro-used subset of the original object
list builder, not an original-code interpreter. The resource callback reads
only art headers, coordinates, sequence-to-art pointers and rotation tables.
Modes 0/2/4/6/8 are normalized into the already-tested `sprites.c` representation;
mode 10 emits nothing. Counts, pointers, class and camera selectors are bounded.
UI mode 4 reads its generated art from native UI-slot RAM, not a ROM sequence.

Queues are visited from layer 14 down to 0, with back UI between layers 8/6
and front UI after layer 0. Each primary draws linked effects at offsets
2e, 2c, then itself, then effects at 2a, 28. Effects must be active and visible.
The original 896-cell budget is distinct from the object-record count. Four
blank priority-zero records and the original terminator are retained for the
construction comparison; the video adapter omits those four invisible records.

Anchors resolve camera origin, optional shake and ten-bit hardware placement.
Rotation coefficients are indirect data, even though the original forms their
address relative to its instruction location. Unsupported row-corrected actors,
gameplay lists and other UI formats fail explicitly. Unobserved regional paths
are not silently replaced with an approximation.

A map clear publishes a blank first object even when no regular presentation
callback runs on that tick. This is a separate pending display event in native
state, not a copied CPU side effect. Original transform register-save scratch
is not game state and is excluded explicitly from state comparisons.

`intro_present_video` maps software register shadows to the shared video
adapter. It performs no drawing or original execution. Cooperative scheduling,
independent initialization, final packaging and sound remain separate work.

## Evidence

Two private construction traversals (different RNG seeds) compare all 2,948
logical updates against original controller/actor/fade/renderer execution:
game state, video memory and every emitted object through the terminator.
The documented headshot non-marker upper-word projection, scheduler task and
CPU register-save scratch are excluded. This is not captured-frame playback,
a game boot or an exact display-clock proof. The title/UI routines participate.

Synthetic controls cover all art modes, transforms, camera/shake, attachments,
UI-generated art, buffer clears, register translation and rejected inputs,
with UBSan and four real mutants. Earlier sprite/video evidence is reused.
See the corresponding reconstruction record for preserved failures and scope.
