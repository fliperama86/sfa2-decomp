/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void reset_and_push_object_to_pool_table(Slab172 *s) {
    if (s->field_02 == 10) {
        if (s->mode != 4) {
            data_801a27cc++;
        }
    }
    if (s->field_02 == 4) {
        data_801a27d4++;
    }
    release_stream_pair_cells(s);
    clear_bytes(s, 0xac);
    s->field_44 = 0;
    s->type_code = 12;
    free_block_count++;
    s->field_26 = 0;
    s->linked_entry_0 = 0;
    s->linked_entry_1 = 0;
    s->linked_entry_2 = 0;
    s->linked_entry_3 = 0;
    table_80197f20[free_block_count] = s;
}
