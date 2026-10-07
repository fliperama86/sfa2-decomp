/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"
void func_8011f240(Slab172 *s);

extern ObjectRef data_80190468;
void func_801e1554_slot0b(Slot0bObj *obj);

void func_801e14b0_slot0b(Slot0bObj *obj) {
    int v;
    if (data_80190468.p->field_ab == 2) {
        v = obj->field_46 - 1;
        obj->field_46 = v;
        if ((s16)v < 0) {
            obj->field_04++;
        }
        func_801e1554_slot0b(obj);
    }
}

void func_801e1514_slot0b(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e1534_slot0b(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e1554_slot0b(Slot0bObj *obj) {
    obj->fx += obj->field_4c;
    obj->field_4c += obj->field_54;
    obj->fy += obj->field_50;
    obj->field_50 += obj->field_58;
}
