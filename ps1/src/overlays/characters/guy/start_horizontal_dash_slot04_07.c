/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void move_x_by_facing_with_decel_stop_slot04_07(Object *obj);
int is_compare_limit_below_threshold_slot04_07(Object *obj);
void call_handler_by_direction_index_slot04_07(Object *obj);

void start_horizontal_dash_slot04_07(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3 = obj->state_3 + 1;
    add_to_meter_clamped(obj, 5);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    obj->vel_x = 0x80000;
    obj->countdown = 0x1e;
    obj->accel_x = 0;
    obj->vel_y = 0;
    obj->accel_y = 0;
    *(s32 *)&((Slot04aObj *)obj)->field_1c0 = -0x8000;
    set_sequence_by_side_and_mode(obj, 0x23);
}

void update_countdown_dash_or_reset_sequence_slot04_07(Object *obj) {
    move_x_by_facing_with_decel_stop_slot04_07(obj);
    if (obj->mode_flag_2 == 0) {
        if (obj->pressed_bits & 0x68) {
            goto call_x;
        }
        obj->countdown = (s16)obj->countdown - 1;
        if ((s16)obj->countdown != 0 && (u16)is_compare_limit_below_threshold_slot04_07(obj) == 0) {
            goto fail;
        }
reset:
        obj->state_3 = 4;
        if (obj->mode_flag_2 != 0) {
            obj->state_3 = 2;
        }
        obj->countdown = 8;
        obj->accel_x = *(s32 *)&((Slot04aObj *)obj)->field_1c0;
        set_sequence_by_side_and_mode(obj, 0x24);
        return;
    }
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown == 0) {
        goto reset;
    }
    if ((u16)is_compare_limit_below_threshold_slot04_07(obj) == 0) {
        goto fail;
    }
call_x:
    call_handler_by_direction_index_slot04_07(obj);
    return;
fail:
    update_sequence(obj);
}
