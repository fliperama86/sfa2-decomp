/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void shift_stream_second_cell_to_first(Slab172 *p) {
    u16 i = p->stream_index;
    free_slot_chain(data_80183ffc[i][0], data_801841bc[i][0]);
    data_80183ffc[i][0] = data_80183ffc[i][1];
    data_801841bc[i][0] = data_801841bc[i][1];
    data_8018437c[i][0] = data_8018437c[i][1];
    data_801841bc[i][1] = 0;
    if (data_80183ffc[i][0] == 0 && data_80183ffc[i][1] == 0) {
        p->stream_index = 0;
    }
    if (data_801841bc[i][0] == 0 && data_801841bc[i][1] == 0) {
        p->stream_index = 0;
    }
}
