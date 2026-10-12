/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void clear_object_and_push_on_free_stack(Object *o) {
    release_stream_pair_cells(o);
    clear_bytes(o, 0xac);
    o->spawn_flag = 0;
    o->mode_id = 0x10;
    data_801a4fec = data_801a4fec + 1;
    o->scroll_factor = 0;
    data_801a68f0[data_801a4fec] = o;
}
