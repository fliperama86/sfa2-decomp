/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern Slot04_07Rec1fdc data_801c1fdc_slot04_07[];
extern Slot04_07Rec1ff4 data_801c1ff4_slot04_07[];

int move_by_velocity_return_new_vel_y_slot04_07(Object *obj);

void update_rising_then_start_next_sequence_slot04_07(Object *obj) {
    int k;

    update_sequence(obj);
    if (!(obj->step_flags & 0x80)) {
        obj->active_state = 1;
        if (move_by_velocity_return_new_vel_y_slot04_07(obj) < 0) {
            if ((u8)obj->step_flags != 0) {
                k = 0;
                if (obj->field_49 != 0) {
                    k = 6;
                }
                obj->vel_x = 0;
                obj->accel_x = 0;
                obj->state_3++;
                set_sequence_by_side_and_mode(obj, data_801c1fdc_slot04_07[(k + obj->direction_index) >> 1].value);
            }
        }
    }
}

void update_fall_land_start_table_sequence_slot04_07(Object *obj) {
    int y;

    update_sequence(obj);
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    y = obj->ground_y;
    obj->vel_y += obj->accel_y;
    if (obj->pos_y < y) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->pos_y = y;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->active_flag_4 = 0;
        play_object_sound_by_kind(obj);
        y = 0;
        if (obj->field_49 != 0) {
            y = 6;
        }
        y += obj->direction_index;
        set_sequence_by_side_and_mode(obj, data_801c1ff4_slot04_07[y >> 1].value);
    }
}
