/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void spawn_linked_pooled_object_health_2_slot04_09(Object *obj);

void update_attack_hit_spawn_linked_object_slot04_09(Object *obj) {
    u16 t;
    update_sequence(obj);
    t = obj->step_flags;
    if ((u8)t == 0) {
        obj->state_3++;
        obj->field_165 = 0;
        obj->other_lock = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 10;
            obj->other_lock = 10;
        }
        spawn_linked_pooled_object_health_2_slot04_09(obj);
        play_side_sound_at(obj, ((Slot04aObj *)obj)->side, 4);
    } else if ((u8)t != 2) {
        obj->step_flags = (t & 0xff00) | 2;
        obj->field_165 = 0xff;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        }
        play_sound_at(obj, ((Slot04aObj *)obj)->side, 0x31c);
        spawn_children_per_direction_index(obj, 0, 0x54);
    }
}

void end_sequence_clear_variant_index_2_slot04_09(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->variant_index_2 = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}
