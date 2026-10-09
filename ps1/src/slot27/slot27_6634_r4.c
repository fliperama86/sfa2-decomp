/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"
extern u16 box_margin[];
extern s16 data_80055eb2;
extern ObjectFn data_80028e74_slot27[];

void func_800169c4_slot27(Object *obj) {
    s16 *p;
    s16 x = obj->pos_x;
    s16 m = *(s16 *)box_margin;
    if (x < m + 0x153) {
        obj->pos_x = x + 8;
        p = &data_80055eb2;
        if (*p > 0x40) {
            *p -= 8;
        }
    } else {
        obj->pos_x = m + 0x153;
        game_state.field_c8 |= 1;
    }
    func_8011ffdc(obj);
}

void func_80016a54_slot27(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_80016a74_slot27(Object *obj) {
    ref_other.p = obj->field_3c;
    (*(Object **)&data_80190458) = obj->other;
    data_80028e74_slot27[obj->field_04](obj);
}
