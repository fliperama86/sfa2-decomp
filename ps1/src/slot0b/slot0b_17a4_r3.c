/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"
void func_8011f240(Slab172 *s);

extern ObjectRef data_80190468;
void func_801e1b40_slot0b(Object *obj);

void func_801e19ec_slot0b(Object *obj) {
    if (data_80190468.p->field_ab != 1) {
        { int t = obj->field_46 - 1; obj->field_46 = t; }
        if ((s16)obj->field_46 >= 0) goto tail;
    }
    obj->field_05++;
    obj->pos_x = obj->field_5c;
tail:
    func_801e1b40_slot0b(obj);
}

void func_801e1a54_slot0b(Object *obj) {
    if (data_80190468.p->field_ab != 2) return;
    obj->field_46 = 0x10;
    obj->field_4c = -0x80000;
    obj->field_54 = 0;
    obj->field_05++;
    if (obj->field_48 != 0) {
        obj->field_4c = -obj->field_4c;
        obj->field_54 = -obj->field_54;
    }
}

void func_801e1ab4_slot0b(Object *obj) {
    { int t = obj->field_46 - 1; obj->field_46 = t; }
    if ((s16)obj->field_46 < 0) {
        obj->field_04++;
    }
    func_801e1b40_slot0b(obj);
}

void func_801e1b00_slot0b(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
