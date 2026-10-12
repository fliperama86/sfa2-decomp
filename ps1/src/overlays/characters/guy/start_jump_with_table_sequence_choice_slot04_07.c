/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern s32 data_801c1f94_slot04_07[];
extern Slot04_07Rec1fc4 data_801c1fc4_slot04_07[];

void start_jump_with_table_sequence_choice_slot04_07(Object *obj) {
    s32 *p;
    s32 a;
    s32 b;

    obj->active_flag_4 = 1;
    obj->state_3++;
    play_side_sound_at(obj, obj->side, 6);
    add_to_meter_clamped(obj, 9);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    p = &data_801c1f94_slot04_07[(obj->direction_index >> 1) * 4];
    a = *p++;
    b = *p++;
    obj->vel_y = p[0];
    obj->accel_y = p[1];
    if (obj->facing_side == 0) {
        obj->vel_x = -a;
        obj->accel_x = -b;
    } else {
        obj->vel_x = a;
        obj->accel_x = b;
    }
    a = 0;
    if (obj->field_49 != 0) {
        a = 6;
    }
    a += obj->direction_index;
    set_sequence_by_side_and_mode(obj, data_801c1fc4_slot04_07[a >> 1].value);
}
