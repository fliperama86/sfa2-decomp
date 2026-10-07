/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"
void func_8011f240(Slab172 *s);

void func_800165a8_slot12(Object *obj) {
    s16 y;

    if (obj->field_48 == 2 && obj->field_03 == 0) {
        obj->field_05 = obj->field_05 + 1;
        return;
    }
    y = obj->pos_y;
    if (!(((Slot12Obj *)obj)->field_78 < y)) {
        obj->field_05 = obj->field_05 + 1;
        if (obj->field_03 == 4) {
            game_state.field_2bc = 8;
        }
    } else {
        obj->pos_y = y - 4;
    }
}

void func_80016634_slot12(Object *obj) {
    if (game_state.field_2bc == 10) {
        obj->field_04 = obj->field_04 + 1;
        obj->field_05 = 0;
    }
}

void func_80016660_slot12(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

extern ObjectRef data_80190468;
extern void (*data_80028a04_slot12[])(Object *);

void func_80016680_slot12(Object *obj) {
    data_80190468.p = (Object *)&game_state;
    data_80028a04_slot12[obj->field_04](obj);
}
