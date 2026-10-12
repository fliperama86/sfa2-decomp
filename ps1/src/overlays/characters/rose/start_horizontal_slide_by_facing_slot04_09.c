/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void apply_velocity_and_acceleration_slot04_09(Object *o);
void update_hit_sequence_with_flag_byte_events_b_slot04_09(Object *obj);

void start_horizontal_slide_by_facing_slot04_09(Object *obj) {
    obj->state_3++;
    obj->vel_y = 0;
    obj->accel_y = 0;
    if (obj->facing_side != 0) {
        obj->vel_x = 0x30000;
        obj->accel_x = 0x2000;
        update_hit_sequence_with_flag_byte_events_b_slot04_09(obj);
    } else {
        obj->vel_x = -0x30000;
        obj->accel_x = -0x2000;
        update_hit_sequence_with_flag_byte_events_b_slot04_09(obj);
    }
}

void update_hit_sequence_with_flag_byte_events_b_slot04_09(Object *obj) {
    u16 t;
    update_sequence(obj);
    t = obj->step_flags;
    if ((u8)t == 0) {
        obj->state_3++;
        obj->step_flags = obj->step_flags & 0xff00;
        obj->other_lock = 0;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 10;
            obj->other_lock = 3;
        }
    } else if ((u8)t != 2) {
        obj->step_flags = (t & 0xff00) | 2;
        obj->field_165 = 0xff;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        }
        play_sound_at(obj, ((Slot04aObj *)obj)->side, 0x31c);
        spawn_children_per_direction_index(obj, -0xc, 0x3f);
    }
}

void move_and_play_side_sound_on_step_flag_slot04_09(Object *obj) {
    apply_velocity_and_acceleration_slot04_09(obj);
    if ((u8)obj->step_flags != 0) {
        obj->state_3++;
        obj->step_flags = obj->step_flags & 0xff00;
        play_side_sound_at(obj, ((Slot04aObj *)obj)->side, 4);
    }
    update_sequence(obj);
}
