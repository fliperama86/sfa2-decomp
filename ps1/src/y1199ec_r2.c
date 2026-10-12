/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void release_stream_pair_cells(AnimObj *obj) {
    free_slot_chain(data_80183ffc[obj->stream_index][0], data_801841bc[obj->stream_index][0]);
    free_slot_chain(data_80183ffc[obj->stream_index][1], data_801841bc[obj->stream_index][1]);
    data_801841bc[data_80183ffc[obj->stream_index][0]][0] = 0;
    data_801841bc[data_80183ffc[obj->stream_index][1]][0] = 0;
    data_801841bc[data_80183ffc[obj->stream_index][0]][1] = 0;
    data_801841bc[data_80183ffc[obj->stream_index][1]][1] = 0;
    data_8018437c[data_80183ffc[obj->stream_index][0]][0] = 0;
    data_8018437c[data_80183ffc[obj->stream_index][1]][0] = 0;
}
