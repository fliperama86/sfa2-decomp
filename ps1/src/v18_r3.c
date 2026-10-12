/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void reset_and_push_block_object_type_8(Slab172 *o) {
    release_stream_pair_cells(o);
    clear_bytes(o, 0xc0);
    o->field_74 = 0;
    o->field_67 = 0;
    o->field_44 = 0;
    o->type_code = 8;
    data_801a6960 = data_801a6960 + 1;
    o->field_26 = 0;
    o->linked_entry_0 = 0;
    o->linked_entry_1 = 0;
    o->linked_entry_2 = 0;
    o->linked_entry_3 = 0;
    o->field_90 = 0;
    data_801a89b0[data_801a6960] = o;
}
