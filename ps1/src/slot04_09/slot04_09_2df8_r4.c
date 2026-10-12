/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_attack_spawn_then_advance_state_3_slot04_09(Object *obj) {
    u16 t;

    update_sequence(obj);
    t = obj->step_flags;
    if ((u8)t == 0) {
        obj->state_3++;
        obj->other_lock = 0;
        if (obj->field_165 & 0x80) {
            obj->other->field_6b = 10;
            obj->other_lock = 2;
        }
        obj->field_165 = 0;
    } else if ((u8)t != 2) {
        obj->step_flags = (t & 0xff00) | 2;
        obj->field_165 = 0xff;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        }
        play_sound_at(obj, obj->side, 0x31c);
        spawn_children_per_direction_index(obj, -0x19, 0x31);
    }
}
