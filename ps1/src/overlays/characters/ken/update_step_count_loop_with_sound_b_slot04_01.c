/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int move_with_gravity_and_check_ground_slot04_01(Object *obj);

void update_step_count_loop_with_sound_b_slot04_01(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        play_sound_at(obj, obj->side, 0x320);
        ((Slot04aObj *)obj)->step_count += 0xff;
        if (((Slot04aObj *)obj)->step_count & 0x80) {
            obj->state_3++;
            set_sequence_by_side_and_mode(obj, 0x21);
            return;
        }
    }
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    update_sequence(obj);
}

void update_jump_arc_land_then_update_sequence_slot04_01(Object *obj) {
    if (move_with_gravity_and_check_ground_slot04_01(obj) != 0) {
        play_object_sound_by_kind(obj);
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
        obj->state_3++;
        set_sequence_by_side(obj, 0x11);
    }
    update_sequence(obj);
}
