/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c43c0_slot04_04[];

void spawn_children_when_countdown_high_byte_set_slot04_04(Object *o) {
    o->countdown = (s16)o->countdown - 0x100;
    if (o->countdown & 0xff00) {
        o->state_3++;
        o->countdown = ((Slot04bObj *)o)->countdown + 0x3800;
        if (o->field_4b != 0) {
            o->field_165 = 1;
        } else {
            o->field_165 = 0xff;
        }
        play_side_sound_at(o, o->side, 6);
        play_sound_at(o, o->side, 0x31c);
        spawn_children_per_direction_index(o, -0xc, 0x30);
    }
}

void count_down_then_set_table_value_sound_7_slot04_04(Object *o) {
    o->countdown = (s16)o->countdown - 0x100;
    if ((o->countdown & 0xff00) == 0) {
        o->state_3++;
        if (o->field_4b != 0) {
            ((Slot04bObj *)o)->table_value = data_801c43c0_slot04_04[3];
        } else {
            ref_other.p = o->other;
            ref_other.p->field_6b = 10;
            ((Slot04bObj *)o)->table_value = data_801c43c0_slot04_04[o->direction_index >> 1];
        }
        o->field_165 = 0;
        play_side_sound_at(o, o->side, 7);
    }
}

void update_sequence_set_active_on_flag_slot04_04(Object *o) {
    if (*(u8 *)&o->step_flags == 0) {
        o->active_state = 1;
        o->state_3++;
    }
    update_sequence(o);
}
