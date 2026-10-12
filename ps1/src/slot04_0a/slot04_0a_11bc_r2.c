/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3e44_slot04_0a(Object *obj);
void func_801b148c_slot04_0a(Object *obj);

int func_801b1320_slot04_0a(Object *obj) {
    int r = 0;

    if (obj->field_7e != 0 || obj->link_flags == 0) {
        if (func_801417cc(obj) != 0) {
            r = 1;
            obj->state_0 = 1;
            obj->state_1 = 0;
            obj->state_2 = 7;
            obj->state_3 = 0;
            obj->field_15a = 2;
            func_801b3e44_slot04_0a(obj);
            obj->active_flag_3 = 1;
            obj->facing_side = obj->saved_facing;
            func_80142b3c(obj);
        }
    }
    return r;
}

int func_801b13c8_slot04_0a(Object *obj) {
    int r = 0;

    if (func_801417cc(obj) != 0) {
        r = 1;
        obj->field_15a = 3;
        ((Slot04aObj *)obj)->offset_index = 0;
        func_801b3e44_slot04_0a(obj);
        func_801b148c_slot04_0a(obj);
    }
    return r;
}

int func_801b1428_slot04_0a(Object *obj) {
    int r = 0;

    if (func_801417cc(obj) != 0) {
        r = 1;
        obj->field_15a = 3;
        ((Slot04aObj *)obj)->offset_index = 4;
        func_801b3e44_slot04_0a(obj);
        func_801b148c_slot04_0a(obj);
    }
    return r;
}
