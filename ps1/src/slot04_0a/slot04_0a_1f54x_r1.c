/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u16 apply_velocity_and_check_ground_slot04_0a(Object *obj);
void nudge_pos_x_from_input_bits_slot04_0a(Object *obj);

void move_airborne_until_landing_then_land_slot04_0a(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        update_sequence(obj);
        return;
    }
    nudge_pos_x_from_input_bits_slot04_0a(obj);
    if (apply_velocity_and_check_ground_slot04_0a(obj)) {
        obj->state_3++;
        play_sound_at(obj, obj->side, 0x314);
        obj->pos_y = (u16)obj->ground_y;
        obj->pos_y_frac = 0;
        obj->active_flag_4 = 0;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x48);
        return;
    }
    update_sequence(obj);
}
