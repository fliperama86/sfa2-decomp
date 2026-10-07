/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Prim data_800f77b4_slot0f[];
extern Slot12Prim data_800f78f4_slot0f[];
extern Slot12Prim data_800f7a34_slot0f[];
extern void (*data_800eff4c_slot0f[])(Object *);
extern ObjectRef data_80190468;
void func_800e5be4_slot0f(Object *obj, void *cell);
void func_8011f240(Slab172 *s);

void func_800e6620_slot0f(Object *obj) {
    Slot12Prim *t;

    switch (obj->field_48) {
    case 0:
        t = data_800f77b4_slot0f;
        break;
    case 1:
        t = data_800f78f4_slot0f;
        break;
    case 2:
        t = data_800f7a34_slot0f;
        break;
    default:
        goto skip;
    }
    func_800e5be4_slot0f(obj, (Slot12Prim *)(obj->field_03 * 64 + (data_801a27d0 * 32 + (u32)t)));
skip:
    if (game_state.field_2bc == 10) {
        obj->field_04++;
        obj->field_05 = 0;
    }
}

void func_800e66ec_slot0f(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_800e670c_slot0f(Object *obj) {
    data_80190468.p = (Object *)&game_state;
    data_800eff4c_slot0f[obj->field_04](obj);
}
