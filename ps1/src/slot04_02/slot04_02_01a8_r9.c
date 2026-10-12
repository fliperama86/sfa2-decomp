/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b631c_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);
void func_801b0fd8_slot04_02(Object *obj);

u8 func_801b0ef4_slot04_02(Object *obj) {
    u8 r = func_801417cc(obj);
    u8 t;

    if (r) {
        func_801b631c_slot04_02(obj, 1, 0, 7, 0);
        t = obj->saved_facing;
        obj->field_15a = 8;
        obj->facing_side = t;
        obj->variant_index = t ^ 1;
        func_801b0fd8_slot04_02(obj);
    }
    return r;
}

u8 func_801b0f68_slot04_02(Object *obj) {
    u8 r = func_801417cc(obj);

    if (r) {
        func_801b631c_slot04_02(obj, 1, 0, 7, 0);
        obj->field_15a = 8;
        obj->variant_index = obj->facing_side = obj->saved_facing;
        func_801b0fd8_slot04_02(obj);
    }
    return r;
}

void func_801b0fd8_slot04_02(Object *obj) {
    u16 t;

    t = obj->pressed_bits | obj->released_bits;
    obj->input_dir_second = 0;
    obj->direction_index = 0;
    if ((t & 0x68) == 0x68 || (t & 2) != 0) {
        obj->input_dir_second = obj->input_dir_second + 2;
    }
}
