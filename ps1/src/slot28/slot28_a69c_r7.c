/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001b1c4_slot28(Object *o);
void func_8001af9c_slot28(Object *o);

void func_8001af40_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    obj->field_47 = 0x20;
    o->vel_x = 0x50000;
    o->vel_y = 0x50000;
    o->accel_y = -0x1000;
    o->field_5c = 3;
    o->state_1++;
    *(u16 *)&o->field_70 = *(u16 *)&o->pos_y;
    o->accel_x = 0;
    o->field_5e = 0;
    func_8001af9c_slot28(o);
}

void func_8001af9c_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    func_8001b1c4_slot28(o);
    if (o->pos_y >= o->field_70) {
        o->state_1++;
        play_sound_at(0, 0, 0x301);
        obj->field_47 = 2;
        o->vel_x = 0x48000;
        o->accel_x = 0x1000;
        o->vel_y = 0x60000;
        o->accel_y = -0x1000;
        *(u16 *)&o->pos_y = *(u16 *)&o->field_70;
    }
}

void func_8001b028_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    func_8001b1c4_slot28(o);
    if (o->pos_y >= o->field_70) {
        obj->field_47 = 1;
        o->vel_x = 0x30000;
        o->vel_y = 0x30000;
        o->accel_y = -0x1000;
        o->state_1++;
        o->accel_x = 0;
        *(u16 *)&o->pos_y = *(u16 *)&o->pos_y + *(u16 *)&o->field_70;
    }
}
