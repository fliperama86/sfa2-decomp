/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

u16 apply_velocity_and_check_ground_slot04_0a(Object *obj);
void land_on_ground_set_state_3_seven_slot04_0a(Object *obj);

void move_airborne_until_vel_y_falls_slot04_0a(Object *obj) {
    if (apply_velocity_and_check_ground_slot04_0a(obj)) {
        land_on_ground_set_state_3_seven_slot04_0a(obj);
    } else if (obj->vel_y > 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x25);
    }
}

void move_airborne_until_active_flag_slot04_0a(Object *obj) {
    if (apply_velocity_and_check_ground_slot04_0a(obj)) {
        land_on_ground_set_state_3_seven_slot04_0a(obj);
    } else if (obj->active_flag == 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->other->field_6b = 0x10;
        set_sequence_by_side_and_mode(obj, 0x23);
    }
}

void land_on_ground_set_state_3_seven_slot04_0a(Object *obj) {
    obj->state_3 = 7;
    play_object_sound_by_kind(obj);
    obj->pos_y_frac = 0;
    obj->pos_y = (u16)obj->ground_y;
    set_sequence_by_side_and_mode(obj, 0x48);
}
