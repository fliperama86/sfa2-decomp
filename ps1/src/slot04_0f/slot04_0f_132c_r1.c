/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80141e34(Object *object);
void func_80142718(Object *object);
void func_80142778(Object *object);

int func_801b132c_slot04_0f(Object *obj) {
    if (obj->field_292 != 0) return 0;
    if ((s16)obj->meter < 0x30) return 0;
    if (obj->active_state == 0) {
        if (!func_80141e34(obj)) return 0;
        if (!(u8)func_80141788(obj)) return 0;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 4;
        obj->facing_side = obj->saved_facing;
    }
    if (obj->field_7e != 0) return 0;
    if (!(u8)func_801418bc(obj)) return 0;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 8;
    obj->state_3 = 0;
    obj->field_15a = 4;
    return 1;
}

int func_801b1418_slot04_0f(Object *obj) {
    if (obj->link_flags != 0) return 0;
    if ((s16)obj->meter < 0x30) return 0;
    if (!(u8)func_80141788(obj)) return 0;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 8;
    obj->state_3 = 0;
    obj->field_15a = 7;
    obj->active_flag_3 = 1;
    obj->facing_side = obj->saved_facing;
    func_80142718(obj);
    return 1;
}

int func_801b14b0_slot04_0f(Object *obj) {
    if ((s16)obj->meter < 0x30) return 0;
    if (!(u8)func_80141788(obj)) return 0;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 8;
    obj->state_3 = 0;
    obj->field_15a = 8;
    obj->facing_side = obj->saved_facing;
    func_80142778(obj);
    return 1;
}
