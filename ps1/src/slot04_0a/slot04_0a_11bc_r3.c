/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3e44_slot04_0a(Object *obj);
void func_801b148c_slot04_0a(Object *obj) {
    int k;

    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->direction_index = 0;
    k = ((obj->pressed_bits | obj->released_bits) & 0x6a) != 0;
    ((Slot04aObj *)obj)->offset_index += k << 1;
}

int func_801b14d0_slot04_0a(Object *obj) {
    int r = 0;

    if ((s16)obj->meter >= 0x30 && (u8)func_80141788(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 4;
        func_801b3e44_slot04_0a(obj);
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        func_80142718(obj);
    }
    return r;
}

int func_801b156c_slot04_0a(Object *obj) {
    int r = 0;

    if ((s16)obj->meter >= 0x30 && (u8)func_80141788(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 5;
        func_801b3e44_slot04_0a(obj);
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        func_80142778(obj);
    }
    return r;
}
