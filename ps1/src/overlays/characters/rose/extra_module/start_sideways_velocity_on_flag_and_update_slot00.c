/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

void start_sideways_velocity_on_flag_and_update_slot00(Object *o) {
    Slot00Obj *obj = (Slot00Obj *)o;
    if (obj->active_flag != 0) {
        o->state_3++;
        play_sound_at(o, o->side, 0x324);
        if (o->facing_side != 0) {
            o->vel_x = 0xa0000;
            o->accel_x = -0x8000;
        } else {
            o->vel_x = 0xfff60000;
            o->accel_x = 0x8000;
        }
    }
    update_sequence(o);
}

void update_slide_push_other_object_slot00(Object *o) {
    Slot00Obj *obj = (Slot00Obj *)o;
    obj->pos_x_hi += o->vel_x;
    o->vel_x += o->accel_x;
    if (o->vel_x == 0) {
        o->state_3++;
        o->pos_x_frac = 0;
    }
    if ((game_state.frame_counter & 3) == 0) {
        if (spawn_random_offset_object_variant_zero(o) & 0xff) {
            if (o->facing_side == 0) {
                ref_other.p->pos_x -= 0x30;
            } else {
                ref_other.p->pos_x += 0x30;
            }
        }
    }
    update_sequence(o);
}
