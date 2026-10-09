/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8007ef34_slot2b;

void func_800795b4_slot2b(Object *obj);
void func_800795f8_slot2b(Object *obj);
void func_800796e4_slot2b(Object *obj);
void func_80079594_slot2b(Object *obj);

void func_800794e0_slot2b(Object *o) {
    func_800795b4_slot2b(o);
    if (data_8007ef34_slot2b->field_70 < o->pos_y) {
        o->field_04++;
        o->pos_y = (u16)data_8007ef34_slot2b->field_70;
        o->field_14 = 0;
        func_800795f8_slot2b(o);
        func_800796e4_slot2b(o);
        func_800796e4_slot2b(o);
    } else {
        func_80131094(o);
    }
}

void func_80079574_slot2b(Object *o) {
    func_80079594_slot2b(o);
}

void func_80079594_slot2b(Object *o) {
    func_8011f240((Slab172 *)o);
}

void func_800795b4_slot2b(Object *o) {
    Slot2bObj *obj = (Slot2bObj *)o;
    obj->field_10 += o->field_4c;
    o->field_4c += o->field_54;
    obj->field_14 -= o->field_50;
    o->field_50 += o->field_58;
}
