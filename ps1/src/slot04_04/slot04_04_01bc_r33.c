/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c43dc_slot04_04[];

void func_801b2b8c_slot04_04(Object *o) {
    o->state_3++;
    func_801428e4(o);
    set_flag_225_add_direction_to_counter(&game_state, o);
    func_80145d20(o);
    set_sequence_by_side_and_mode(o, 0x59);
}

void func_801b2be8_slot04_04(Object *o) {
    update_sequence(o);
    if (*(u8 *)&o->step_flags != 0) {
        o->state_3++;
        if (o->field_4b != 0) {
            o->field_165 = 1;
        } else {
            o->field_165 = 0xff;
        }
        play_side_sound_at(o, o->side, 8);
        play_sound_at(o, o->side, 0x31c);
        spawn_children_per_direction_index(o, -0x1a, 0x33);
    }
}

void set_table_value_on_sequence_end_sound_9_slot04_04(Object *o) {
    u8 t;

    update_sequence(o);
    if (*(u8 *)&o->step_flags == 0) {
        o->state_3++;
        if (o->field_4b != 0) {
            t = 3;
        } else {
            ref_other.p = o->other;
            ref_other.p->field_6b = 10;
            t = o->direction_index >> 1;
        }
        ((Slot04bObj *)o)->table_value = data_801c43dc_slot04_04[t];
        o->field_165 = 0;
        play_side_sound_at(o, o->side, 9);
    }
}
