/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801c8bfc_slot05_06(Object *obj) {
    if (!func_801417cc(obj)) return 0;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 0;
    obj->facing_side = obj->saved_facing;
    func_80142b3c(obj);
    return 1;
}

int func_801c8c64_slot05_06(Object *obj) {
    if ((s16)obj->meter < 0x30) return 0;
    if (!(u8)func_80141788(obj)) return 0;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 8;
    obj->state_3 = 0;
    obj->facing_side = obj->saved_facing;
    obj->field_15a = 1;
    obj->active_flag_3 = 1;
    obj->direction_index = 4;
    func_80142718(obj);
    return 1;
}

int func_801c8cf0_slot05_06(Object *obj) {
    if (!func_801417cc(obj)) return 0;
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 2;
    obj->active_flag_3 = 1;
    obj->facing_side = obj->saved_facing;
    func_80142b3c(obj);
    return 1;
}
