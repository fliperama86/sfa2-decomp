/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int is_not_state_2_five(Object *object);
void func_80142718(Object *object);
void func_80142778(Object *object);

int try_start_meter_special_state_8_type_4_slot04_0f(Object *obj) {
    if (obj->field_292 != 0) return 0;
    if ((s16)obj->meter < 0x30) return 0;
    if (obj->active_state == 0) {
        if (!is_not_state_2_five(obj)) return 0;
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

int try_start_meter_special_state_8_type_7_slot04_0f(Object *obj) {
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

int try_start_meter_special_state_8_type_8_slot04_0f(Object *obj) {
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
