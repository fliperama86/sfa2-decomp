/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudSlot data_801e7648_slot0b[];
extern int data_801e4cfc_slot0b[];
void func_801e1860_slot0b(Object *obj, Object *other);
void func_801e1908_slot0b(Object *obj, Object *other);

void func_801e17a4_slot0b(Object *obj) {
    Object *o;
    obj->field_0a = 1;
    obj->field_46 = 0x1f;
    obj->pos_x = -0xb8;
    obj->pos_y = 0xc9;
    obj->field_5c = 0x48;
    obj->field_4c = 0x80000;
    obj->field_0e = 0;
    obj->field_09 = 0;
    obj->field_24 = 0;
    obj->field_0b = 0;
    obj->field_20 = 0;
    obj->field_22 = 0;
    obj->field_0c = 0;
    obj->field_54 = 0;
    obj->field_04++;
    if (obj->field_48 != 0) {
        obj->pos_x = 0x228;
        obj->field_5c = 0x128;
        obj->field_4c = -obj->field_4c;
        obj->field_54 = -obj->field_54;
    }
    o = obj->field_3c;
    ref_other.p = o;
    func_801e1860_slot0b(obj, o);
}

void func_801e1860_slot0b(Object *obj, Object *other) {
    HudSlot *s = &data_801e7648_slot0b[other->side];
    s->field_08 = 0x10;
    s->field_09 = 0x10;
    s->field_0a = 2;
    s->field_00 = 0;
    s->field_0b = 0x18;
    s->field_0c = data_801e4cfc_slot0b[other->kind];
    func_801e1908_slot0b(obj, other);
}
