/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void set_random_sequence_from_variant(Object *object, int a) {
    int base = get_random_byte() & 0xf;
    int off = a * 2 + 0x30;
    set_sequence_from_table_5(object, (table_801802cc[base] + off) & 0xff, (SequenceStep **)table_8017c7f8);
}
