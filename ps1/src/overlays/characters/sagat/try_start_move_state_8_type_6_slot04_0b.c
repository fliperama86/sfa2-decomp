/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int try_start_move_state_8_type_6_slot04_0b(Object *obj) {
    int r = 0;

    if ((s16)obj->meter >= 0x30) {
        if (obj->link_flags != 0) {
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
            obj->field_6b = 0;
            obj->facing_side = obj->saved_facing;
            set_direction_index_from_buttons_mask_a(obj);
        }
    }
    return r;
}
