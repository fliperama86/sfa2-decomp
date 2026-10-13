/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void set_capped_value_and_ot_indices(ObjectView *object) {
    u32 v;
    u8 f = object->packed_value;
    if (f & 1) {
        v = f >> 2;
        if (v > 0x10) {
            v = 0x10;
        }
    } else {
        v = 0;
    }
    object->capped_value = v;
    data_8017262c[game_state.kind_index]();
    background_layers[0].ot_index = data_801725f0[object->table_index * 3 + 0];
    background_layers[1].ot_index = data_801725f0[object->table_index * 3 + 1];
    background_layers[2].ot_index = data_801725f0[object->table_index * 3 + 2];
    data_80190562[0] = data_801725f0[object->table_index * 3 + 1];
}
