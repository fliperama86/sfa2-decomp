/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern s32 data_801bef6c_slot04_01[];

void start_move_with_table_velocity_pair_slot04_01(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    int h;
    int a = 0x1d;

    o->active_flag_4 = 1;
    o->state_3++;
    play_side_sound_at(o, obj->side, 0xb);
    add_to_meter_clamped(o, 9);
    add_one_to_ec4_if_object_unflagged(&game_state, o);
    o->accel_x = -0x8000;
    o->accel_y = -0x6000;
    h = o->direction_index >> 1;
    o->vel_x = data_801bef6c_slot04_01[h * 2];
    o->vel_y = data_801bef6c_slot04_01[h * 2 + 1];
    if (o->field_49 != 0) {
        a = 0x42;
    }
    set_sequence_by_side_and_mode(o, h + a);
}

void update_sequence_or_advance_state_3_slot04_01(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    if (obj->mode != 0) {
        update_sequence(o);
    } else {
        o->active_state = 1;
        o->state_3++;
    }
}
