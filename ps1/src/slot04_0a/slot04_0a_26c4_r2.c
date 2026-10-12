/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void clear_kind10_flag_and_call_helper_slot04_0a(Object *obj);

void move_by_vel_x_until_countdown_or_bound_slot04_0a(Object *obj) {
    u8 t;
    Slot04aObj *s = (Slot04aObj *)obj;
    if (obj->active_flag != 0) {
        obj->active_flag = 0;
        t = s->countdown;
        if ((t & 0x80) == 0) {
            s->countdown = t - 1;
            if (s->countdown & 0x80) {
                set_sequence_by_side_and_mode(obj, 0x42);
            }
        }
    }
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown >= 0 && obj->facing_side != obj->bound_push_flag - 1) {
        *(s32 *)&obj->pos_x_frac += obj->vel_x;
        update_sequence(obj);
    } else {
        obj->state_3++;
        clear_kind10_flag_and_call_helper_slot04_0a(obj);
        set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x43);
    }
}

void finish_sequence_restore_pos_y_and_reset_slot04_0a(Object *obj) {
    if (((s16)obj->step_flags & 0x8000) == 0) {
        update_sequence(obj);
    } else {
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->pos_y = ((Slot04aObj *)obj)->saved_pos_y;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}
