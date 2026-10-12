/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e1b40_slot0b(Object *obj);

void func_801e19ec_slot0b(Object *obj) {
    if (data_80190468.p->field_ab != 1) {
        { int t = obj->field_46 - 1; obj->field_46 = t; }
        if ((s16)obj->field_46 >= 0) goto tail;
    }
    obj->state_1++;
    obj->pos_x = obj->health;
tail:
    func_801e1b40_slot0b(obj);
}

void func_801e1a54_slot0b(Object *obj) {
    if (data_80190468.p->field_ab != 2) return;
    obj->field_46 = 0x10;
    obj->vel_x = -0x80000;
    obj->accel_x = 0;
    obj->state_1++;
    if (obj->field_48 != 0) {
        obj->vel_x = -obj->vel_x;
        obj->accel_x = -obj->accel_x;
    }
}

void func_801e1ab4_slot0b(Object *obj) {
    { int t = obj->field_46 - 1; obj->field_46 = t; }
    if ((s16)obj->field_46 < 0) {
        obj->state_0++;
    }
    func_801e1b40_slot0b(obj);
}

void func_801e1b00_slot0b(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
