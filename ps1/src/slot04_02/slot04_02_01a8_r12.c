/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b631c_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

u8 func_801b1314_slot04_02(Object *obj) {
    u8 r = func_801417cc(obj);

    if (r) {
        func_801b631c_slot04_02(obj, 1, 0, 7, 0);
        obj->field_15a = 0;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        func_80142b3c(obj);
    }
    return r;
}

u8 func_801b1384_slot04_02(Object *obj) {
    u8 r;
    int t;

    if (obj->field_7e == 0 && obj->link_flags != 0) return 0;
    if (obj->active_state != 0) {
        r = func_801418bc(obj);
        if (!r) return;
        t = (u16)obj->ground_y;
        t -= 0x30;
        if (obj->pos_y >= (s16)t) return 0;
        obj->field_15a = 4;
        obj->active_flag_3 = 1;
        func_801b631c_slot04_02(obj, 1, 0, 7, 0);
        func_80142b3c(obj);
    } else {
        r = func_801417cc(obj);
        if (r) {
            obj->field_15a = 1;
            obj->active_flag_3 = 1;
            obj->facing_side = obj->saved_facing;
            func_801b631c_slot04_02(obj, 1, 0, 7, 0);
            func_80142b3c(obj);
        }
    }
    return r;
}
