/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801417cc(Object *object);
/* function of another unit of this module */
void func_801b62a8_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d);
void func_80142718(Object *object);
void func_80142ba0(Object *object);

u8 func_801b1558_slot04_14(Object *obj) {
    u8 r;

    if (obj->active_state != 0) {
        r = func_801418bc(obj);
        if (!r) return;
        obj->field_15a = 3;
        func_801b62a8_slot04_14(obj, 1, 0, 7, 0);
        obj->active_flag_3 = 1;
        func_80142ba0(obj);
    } else {
        r = func_801417cc(obj);
        if (!r) return;
        obj->field_15a = 2;
        obj->facing_side = obj->saved_facing;
        func_801b62a8_slot04_14(obj, 1, 0, 7, 0);
        obj->active_flag_3 = 1;
        func_80142ba0(obj);
    }
    return r;
}

u8 func_801b1614_slot04_14(Object *obj) {
    u8 r = 0;

    if ((s16)obj->meter >= 0x30) {
        if (obj->link_flags != 0) return 0;
        r = func_80141788(obj);
        if (r) {
            func_801b62a8_slot04_14(obj, 1, 0, 8, 0);
            obj->field_15a = 5;
            obj->active_flag_3 = 1;
            obj->facing_side = obj->saved_facing;
            func_80142718(obj);
        }
    }
    return r;
}
