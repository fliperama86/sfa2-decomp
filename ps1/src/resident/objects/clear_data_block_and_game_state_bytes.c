/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void clear_data_block_and_game_state_bytes(void) {
    u8 *p = &data_801ae02c;
    u32 i;
    for (i = 0; i < 0xac; i++) {
        *p++ = 0;
    }
    (&data_801ae02c)[8] = 0x18;
    game_state.busy_flag = 0;
    game_state.field_04 = 0;
    game_state.pause_flag = 0;
}

/* The parameter is unused here. It is declared because calls found so far, in character files of slot 0x4, set the first argument register in the delay slot of the call; that is compatible with a parameter and does not prove one. */
Object *pop_object_from_pool(Object *unused) {
    Object *o;
    Object *result;
    int n = data_801a6960;
    if (n != 0) {
        o = data_801a89b0[n];
        data_801a6960 = n - 1;
        o->offset_index = 0;
        clear_anim_obj_stream_index((AnimObj *)o);
        result = o;
    } else {
        result = 0;
    }
    return result;
}
