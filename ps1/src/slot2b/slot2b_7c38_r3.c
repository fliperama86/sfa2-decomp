/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80079968_slot2b[];

void call_handler_by_state_1_and_check_offscreen_slot2b(Object *o) {
    data_80079968_slot2b[o->state_1](o);
    if (game_state.frame_counter & 1) {
        clear_or_set_flag_by_pos_x_limit(o);
    }
}

void release_from_parent_and_launch_or_reset_slot2b(Object *o) {
    Object *p = o->parent;
    o->state_1++;
    p->linked_object_3 = 0;
    if (o->active_flag != 0) {
        o->state_0 = 3;
        o->state_1 = 0;
        o->state_2 = 0;
        o->state_3 = 0;
    } else {
        o->vel_y = 0xa0000;
        o->accel_y = 0xffff0000;
        set_sequence_by_sound_side(o, 3);
        play_side_sound_at(o, p->side, 0x17);
    }
}
