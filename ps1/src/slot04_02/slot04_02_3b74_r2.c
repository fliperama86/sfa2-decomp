/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_countdown_spawn_children_when_mode_set_slot04_02(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    o->countdown = (s16)o->countdown - 0x100;
    if (obj->mode != 0) {
        o->state_3++;
        if (o->field_4b != 0) {
            o->field_165 = 1;
        } else {
            o->field_165 = 0xff;
        }
        o->countdown = (o->countdown & 0xff00) | 0x30;
        if (o->facing_side != 0) {
            o->vel_x = 0x48000;
        } else {
            o->vel_x = -0x48000;
        }
        play_sound_at(o, o->side, 0x31c);
        spawn_children_per_direction_index(o, -0x10, 0x58);
    }
    update_sequence(o);
}

void count_down_high_byte_then_advance_state_3_slot04_02(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    o->countdown = (s16)o->countdown - 0x100;
    if (o->countdown & 0x8000) {
        o->field_165 = 0;
        o->state_3++;
        o->countdown = obj->flag_bits | 0x400;
        play_side_sound_at(o, o->side, 0xc);
    }
    update_sequence(o);
}

void count_down_high_byte_advance_state_3_if_nonzero_slot04_02(Object *o) {
    o->countdown = (s16)o->countdown - 0x100;
    if ((o->countdown & 0xff00) != 0) {
        o->state_3++;
    }
    update_sequence(o);
}
