/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801dd5d0_slot05_06[])(Object *);
extern Object *data_801dd758_slot05_06;
extern ObjectFn data_801dd5e0_slot05_06[];
void func_801ce260_slot05_06(Object *obj);
void func_801ce214_slot05_06(Object *obj);
void func_801ce2c0_slot05_06(Object *obj);

void func_801ce050_slot05_06(Object *obj) {
    data_801dd758_slot05_06 = obj->field_3c;
    data_801dd5d0_slot05_06[obj->field_04](obj);
}

void func_801ce0a0_slot05_06(Object *obj) {
    Object *p;

    obj->field_04++;
    obj->field_0c = data_801dd758_slot05_06->field_0c;
    obj->field_0d = data_801dd758_slot05_06->field_0d;
    p = data_801dd758_slot05_06;
    obj->pos_x = p->pos_x;
    obj->pos_y = p->pos_y;
    obj->field_0e = p->field_0e;
    obj->field_09 = 6;
    obj->field_0b = data_801dd758_slot05_06->field_0b;
    ((Slot04aObj *)obj)->field_70 = ((Slot04aObj *)data_801dd758_slot05_06)->field_70;
    func_801ce260_slot05_06(obj);
    data_801dd5e0_slot05_06[obj->field_03](obj);
    func_801ce214_slot05_06(obj);
}

void func_801ce198_slot05_06(Object *obj) {
    obj->field_09 = 1;
    obj->field_48 = 0;
}

void func_801ce1a8_slot05_06(Object *obj) {
    obj->field_09 = 1;
    obj->field_48 = 0x10;
}

void func_801ce1bc_slot05_06(Object *obj) {
    func_801ce2c0_slot05_06(obj);
}
