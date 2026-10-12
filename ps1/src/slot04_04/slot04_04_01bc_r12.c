/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1510_slot04_04(Object *obj);

u8 func_801b107c_slot04_04(Object *obj) {
    int r = 0;

    if ((s16)obj->meter < 0x30) {
        return 0;
    }
    if ((u8)func_80141788(obj)) {
        r = 1;
        func_801b1510_slot04_04(obj);
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

u8 func_801b111c_slot04_04(Object *obj) {
    int r = 0;

    if ((s16)obj->meter < 0x30) {
        return 0;
    }
    if ((u8)func_80141788(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 6;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        func_80142718(obj);
    }
    return r;
}
