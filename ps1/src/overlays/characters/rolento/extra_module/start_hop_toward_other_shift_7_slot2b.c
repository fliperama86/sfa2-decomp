/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern Object *data_8007ef34_slot2b;
extern ObjectFn data_8007edc8_slot2b[];

void start_hop_toward_other_shift_7_slot2b(Object *o) {
    o->field_0e = data_8007ef34_slot2b->field_0e;
    o->vel_x = (((Slot2bObj *)data_8007ef34_slot2b->other)->pos_x_hi - ((Slot2bObj *)data_8007ef34_slot2b)->pos_x_hi) >> 7;
    o->accel_x = 0;
    o->vel_y = 0x30000;
    o->accel_y = 0xffff6000;
    set_sequence_by_sound_side(o, 4);
}

void start_vertical_hop_slot2b(Object *o) {
    o->field_0e = data_8007ef34_slot2b->field_0e;
    o->vel_x = 0;
    o->accel_x = 0;
    o->vel_y = 0x60000;
    o->accel_y = 0xffff6000;
    set_sequence_by_sound_side(o, 4);
}

void call_handler_by_table_index_unless_gated_slot2b(Object *o) {
    if (game_state.gate_flag != 0) {
        o->state_0 = o->state_0 + 1;
    } else {
        if (game_state.field_65 == 0) {
            data_8007edc8_slot2b[o->table_index](o);
        }
        set_flag_and_push_if_mode_8(o);
    }
}
