/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801c5320_slot04_06[];
extern s32 data_801c532c_slot04_06[];

void resolve_hit_range_sounds_and_sequence_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    int i = o->direction_index >> 1;
    s16 x;

    if (o->field_49 != 0) {
        i += 3;
    }
    x = data_801c5320_slot04_06[i];
    if (o->state_2 == 8) {
        x = 0x60;
    }
    if ((u8)func_8013f8c4(o, -0x20, x)) {
        o->active_flag_3 = 1;
        o->state_3 += 2;
        o->pos_y = obj->saved_pos_y;
        play_side_sound_at(o, o->side, 9);
        play_sound_at(o->other, o->other->side, 0x31a);
        if (o->state_2 == 8) {
            spawn_three_child_records_at_other(o);
        }
        set_sequence_by_side_and_mode(o, 0x1f);
    } else {
        o->state_3++;
        play_side_sound_at(o, o->side, 1);
        set_sequence_by_side_and_mode(o, 0x27);
    }
}

void start_move_with_velocities_from_table_slot04_06(Object *o) {
    s32 *p;

    o->substate_index = 0;
    o->field_12d = 0;
    o->step_count_2 = 0;
    o->step_limit = 0;
    o->state_3++;
    add_to_meter_clamped(o, 5);
    p = &data_801c532c_slot04_06[o->direction_index * 2];
    o->vel_x = *p++;
    o->vel_y = *p++;
    o->accel_x = *p++;
    o->accel_y = *p;
    set_sequence_by_side_and_mode(o, 0x1f);
}
