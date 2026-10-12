/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_80028484_slot12[][4];

void set_base_and_spawn_from_table_slot12(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[24];
    int i = obj->table_index;
    if (i != 5) {
        obj->base_x = data_80028484_slot12[i][0];
        obj->base_y = data_80028484_slot12[i][1];
        obj->spawn_x = data_80028484_slot12[i][2];
        obj->spawn_y = data_80028484_slot12[i][3];
    }
}

void set_health_from_y_offset_and_table_index_slot12(Object *obj) {
    s16 c;
    int k;
    obj->y_offset = data_801903c0;
    if (game_state.mode_flag_2 != 0) {
        obj->y_offset = data_801903c2;
    }
    k = obj->table_index;
    if (k != 5) {
        c = obj->y_offset;
        if (c == 0) goto same;
        if (k == 0) goto copy;
        if (c < k) goto same;
        obj->health = k - 1;
        goto end;
copy:
        obj->health = c;
        goto end;
same:
        obj->health = obj->table_index;
    }
end:;
}
