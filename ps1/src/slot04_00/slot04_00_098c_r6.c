/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b1084_slot04_00(Object *obj) {
    int r = 0;

    if ((s16)obj->meter >= 0x30 && (u8)func_80141788(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 5;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        func_80142778(obj);
    }
    return r;
}

int func_801b1110_slot04_00(Object *obj) {
    int r = 0;

    if (obj->active_state != 0) {
        if (!(u8)func_801418bc(obj)) return 0;
        r = 1;
        obj->field_15a = 3;
    } else {
        if (!func_801417cc(obj)) return 0;
        r = 1;
        obj->field_15a = 2;
        obj->facing_side = obj->saved_facing;
    }
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->active_flag_3 = 1;
    func_80142ba0(obj);
    return r;
}
