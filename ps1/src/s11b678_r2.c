/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

u32 func_8011bdc0(Block172 *block) {
    SequenceStep *seq = block->sequence;
    u32 v;
    u32 base;
    if (seq->flags & 0x8000) {
        v = seq[seq->loop_offset].data_ptr;
    } else {
        v = seq[1].data_ptr;
    }
    base = block->field_9c;
    return ((v >> 1) << 1) + base;
}
