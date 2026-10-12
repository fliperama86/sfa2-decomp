/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_direction_index_from_buttons_mask_b(Object *object);

int try_start_meter_move_eight_slot04_0d(Object *obj) {
    if ((s16)obj->meter < 0x30) goto z;
    if ((u8)func_80141788(obj)) goto b;
z:
    return 0;
b:
    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 8;
    obj->state_3 = 0;
    obj->field_15a = 8;
    obj->active_flag_3 = 1;
    obj->facing_side = obj->saved_facing;
    set_direction_index_from_buttons_mask_b(obj);
    return 1;
}
