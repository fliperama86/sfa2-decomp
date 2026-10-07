/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801903c0;
extern u16 data_801903c2;
void func_8011f240(Slab172 *s);
extern void (*data_800f018c_slot0f[])(Object *);
extern void (*data_800f01a4_slot0f[])(Object *);

void func_800e73bc_slot0f(Object *obj) {
    u16 v;
    s16 c;
    int k;
    obj->field_5e = data_801903c0;
    if (game_state.field_2bd != 0) {
        obj->field_5e = data_801903c2;
    }
    k = obj->field_03;
    if (k != 5) {
        c = obj->field_5e;
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
        v = obj->field_03;
store:
        obj->field_5c = v;
    }
}

void func_800e743c_slot0f(Object *obj) {
    ref_first.p = (Object *)table_8016e5c4;
    if (game_state.field_2bd != 0) {
        ref_first.p = (Object *)table_8016e614;
    }
    data_800f018c_slot0f[obj->field_03](obj);
}

void func_800e74a8_slot0f(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_800e74c8_slot0f(Object *obj) {
    data_800f01a4_slot0f[obj->field_05](obj);
}
