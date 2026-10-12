/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b631c_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

u8 func_801b0c0c_slot04_02(Object *obj) {
    u8 r = 0;

    if (obj->field_292 == 0) {
        if ((s16)obj->meter >= 0x30) {
            if (obj->active_state == 0) {
                if (func_80141e34(obj) & 0xff) {
                    r = func_80141788(obj);
                    if (r) {
                        func_801b631c_slot04_02(obj, 1, 0, 8, 0);
                        obj->field_15a = 0xf;
                        obj->facing_side = obj->saved_facing;
                    }
                }
            } else if (obj->field_7e == 0) {
                r = func_801418bc(obj);
                if (r) {
                    func_801b631c_slot04_02(obj, 1, 0, 8, 0);
                    obj->field_15a = 0xf;
                }
            }
        }
    }
    return r;
}

u8 func_801b0d08_slot04_02(Object *obj) {
    u8 r = func_801417cc(obj);

    if (r) {
        func_801b631c_slot04_02(obj, 1, 0, 7, 0);
        obj->field_15a = 0xe;
        obj->facing_side = obj->saved_facing;
        func_80142b3c(obj);
    }
    return r;
}
