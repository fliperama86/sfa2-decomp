/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_walk_by_input_or_random_slot04_10(Object *obj);
void update_sequence_or_end_move_a_slot04_10(Object *obj);

void update_walk_by_input_or_random_slot04_10(Object *obj) {
    s32 a1;

    if ((s16)obj->step_flags != 0) {
        obj->active_flag_4 = 0;
        obj->state_3 = obj->state_3 + 1;
        update_sequence_or_end_move_a_slot04_10(obj);
        return;
    }
    if (obj->mode_flag_2 == 0) {
        a1 = obj->vel_x;
        if (obj->input_bits & 0x8000) {
        } else if (obj->input_bits & 0x2000) {
            a1 = -a1;
        } else {
            a1 = 0;
        }
        *(s32 *)&obj->pos_x_frac += a1;
        update_sequence(obj);
    } else {
        if (((Slot04bObj *)obj)->random_value_a == 0) {
            update_sequence(obj);
        } else if (((Slot04bObj *)obj)->field_1aa != 0) {
            ((Slot04bObj *)obj)->field_1aa--;
            update_sequence(obj);
        } else {
            if (((Slot04bObj *)obj)->random_countdown == 0) {
                update_sequence(obj);
            }
            a1 = obj->vel_x;
            ((Slot04bObj *)obj)->random_countdown--;
            if (((Slot04bObj *)obj)->random_value_a & 0x80) {
                a1 = -a1;
            }
            if (obj->facing_side != 0) {
                a1 = -a1;
            }
            *(s32 *)&obj->pos_x_frac += a1;
            update_sequence(obj);
        }
    }
}
