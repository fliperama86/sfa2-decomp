/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void spawn_child_table_4_at_offset_slot04_09(Object *obj);
void spawn_two_opposed_moving_children_slot04_09(Object *obj);

void update_sequence_play_sound_flag_1_slot04_09(Object *obj) {
    u16 t = obj->step_flags;

    if ((t & 0xff) == 1) {
        obj->step_flags = t & 0xff00;
        play_side_sound_at(obj, obj->side, 0xd);
    }
    update_sequence(obj);
}

void update_sequence_sound_and_spawn_on_flag_bytes_slot04_09(Object *obj) {
    u16 t = obj->step_flags;
    u16 u;

    if ((t & 0xff) == 1) {
        obj->step_flags = t & 0xff00;
        play_side_sound_at(obj, obj->side, 0xe);
    }
    u = obj->step_flags;
    if ((u & 0xff) == 2) {
        obj->step_flags = u & 0xff00;
        spawn_child_table_4_at_offset_slot04_09(obj);
    }
    update_sequence(obj);
}

void update_sequence_sound_and_spawn_pair_on_flags_slot04_09(Object *obj) {
    u16 t = obj->step_flags;
    u16 u;

    if ((t & 0xff) == 1) {
        obj->step_flags = t & 0xff00;
        play_side_sound_at(obj, obj->side, 0xc);
    }
    u = obj->step_flags;
    if ((u & 0xff) == 2) {
        obj->step_flags = u & 0xff00;
        spawn_two_opposed_moving_children_slot04_09(obj);
    }
    update_sequence(obj);
}
