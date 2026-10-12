/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b1064_slot04_05(Object *obj);

int func_801b0f04_slot04_05(Object *obj) {
    int r = 0;
    u8 one;

    if (func_801417cc(obj) != 0) {
        r = 1;
        one = 1;
        obj->state_0 = one;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 2;
        obj->active_flag_3 = one;
        obj->facing_side = obj->saved_facing;
        func_80142ba0(obj);
    }
    return r;
}

int func_801b0f7c_slot04_05(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    int r = 0;
    int i;
    u8 *p;
    u8 c;

    p = &o->slots[7].stage;
    if (func_801b1064_slot04_05(o)) {
        for (c = r, i = 7; i >= 0; i--) {
            *p++ = c;
        }
        r++;
        obj->field_1a5 = 0x18;
        obj->field_1a6 = 1;
    }
    return r;
}

int func_801b0ff0_slot04_05(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    int r = 0;
    int i;
    u8 *p;
    u8 c;

    p = &o->slots[8].stage;
    if (func_801b1064_slot04_05(o)) {
        for (c = r, i = 7; i >= 0; i--) {
            *p++ = c;
        }
        r++;
        obj->field_1a5 = 0x18;
        obj->field_1a6 = 2;
    }
    return r;
}

int func_801b1064_slot04_05(Object *obj) {
    int r = 0;

    if (obj->state_2 == 8 && obj->field_15a == 5) {
        r = obj->active_state == 0;
    }
    return r;
}
