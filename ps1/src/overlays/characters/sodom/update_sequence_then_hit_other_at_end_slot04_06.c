/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_state_3_dash_velocity_sound_sequence_slot04_06(Object *obj);

void update_sequence_then_hit_other_at_end_slot04_06(Object *obj) {
    Slot04aObj *s = (Slot04aObj *)obj;
    Object *other;

    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else if (obj->direction_index == 4) {
        set_state_3_dash_velocity_sound_sequence_slot04_06(obj);
    } else {
        other = obj->other;
        other->field_15b = 1;
        other->hit_flag = 1;
        set_other_hit_reaction_state_0(obj, 2, 0xf, -0x200, 0, 0, 0);
        if ((s16)other->health < 0) {
            obj->field_167 = (obj->direction_index >> 1) + 0xc;
        }
        obj->vel_x = -0x20000;
        obj->vel_y = 0x44000;
        obj->accel_x = 0;
        obj->accel_y = -0x4800;
        obj->active_state = 1;
        obj->state_3 = obj->state_3 + 1;
        set_sequence_by_side_and_mode(obj, 0x26);
    }
}

void set_state_3_dash_velocity_sound_sequence_slot04_06(Object *obj) {
    Slot04aObj *s = (Slot04aObj *)obj;

    obj->state_3 = 0x13;
    obj->vel_x = 0x80000;
    obj->accel_x = 0;
    play_sound_at((Object *)&obj->other, s->sound_side, 0x34e);
    set_sequence_by_side_and_mode(obj, 0x38);
}
