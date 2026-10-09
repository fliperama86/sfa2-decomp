/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001b1c4_slot28(Object *o);
void func_8001af9c_slot28(Object *o);

void func_8001af40_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    obj->field_47 = 0x20;
    o->field_4c = 0x50000;
    o->field_50 = 0x50000;
    o->field_58 = -0x1000;
    o->field_5c = 3;
    o->field_05++;
    *(u16 *)&o->field_70 = *(u16 *)&o->pos_y;
    o->field_54 = 0;
    o->field_5e = 0;
    func_8001af9c_slot28(o);
}

void func_8001af9c_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    func_8001b1c4_slot28(o);
    if (o->pos_y >= o->field_70) {
        o->field_05++;
        func_80120554(0, 0, 0x301);
        obj->field_47 = 2;
        o->field_4c = 0x48000;
        o->field_54 = 0x1000;
        o->field_50 = 0x60000;
        o->field_58 = -0x1000;
        *(u16 *)&o->pos_y = *(u16 *)&o->field_70;
    }
}

void func_8001b028_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    func_8001b1c4_slot28(o);
    if (o->pos_y >= o->field_70) {
        obj->field_47 = 1;
        o->field_4c = 0x30000;
        o->field_50 = 0x30000;
        o->field_58 = -0x1000;
        o->field_05++;
        o->field_54 = 0;
        *(u16 *)&o->pos_y = *(u16 *)&o->pos_y + *(u16 *)&o->field_70;
    }
}
