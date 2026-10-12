/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800f018c_slot0f[])(Object *);
extern void (*data_800f01a4_slot0f[])(Object *);

void func_800e73bc_slot0f(Object *obj) {
    u16 v;
    s16 c;
    int k;
    obj->y_offset = data_801903c0;
    if (game_state.mode_flag_2 != 0) {
        obj->y_offset = data_801903c2;
    }
    k = obj->table_index;
    if (k != 5) {
        c = obj->y_offset;
        v = c;
        if (c != 0) {
            if (k == 0) {
                goto store;
            }
            if (k >= c) {
                v = k - 1;
                goto store;
            }
        }
        v = obj->table_index;
store:
        obj->health = v;
    }
}

void func_800e743c_slot0f(Object *obj) {
    ref_first.p = (Object *)table_8016e5c4;
    if (game_state.mode_flag_2 != 0) {
        ref_first.p = (Object *)table_8016e614;
    }
    data_800f018c_slot0f[obj->table_index](obj);
}

void func_800e74a8_slot0f(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_800e74c8_slot0f(Object *obj) {
    data_800f01a4_slot0f[obj->state_1](obj);
}
