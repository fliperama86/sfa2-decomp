/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800374a4_slot28[])(Object *);

void func_8001b0f0_slot28(Object *obj) {
}

void func_8001b0f8_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    obj->field_47--;
    if (obj->field_47 == 0) {
        o->state_1++;
    }
}

void func_8001b130_slot28(Object *obj) {
    data_800374a4_slot28[obj->state_1](obj);
    obj->field_01 = 1;
}
