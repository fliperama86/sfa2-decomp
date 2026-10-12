/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void move_x_by_facing_with_decel_slot04_03(Object *obj);

extern u8 data_801c0674_slot04_03[];

void update_sequence_with_flag_triggered_sound_step_slot04_03(Object *obj) {
    u8 i;

    update_sequence(obj);
    if ((s16)obj->step_flags & 0xff00) {
        i = 0;
        obj->state_3++;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
            i = (obj->direction_index >> 1) + 1;
        }
        obj->other_lock = data_801c0674_slot04_03[i];
        play_side_sound_at(obj, obj->side, 5);
        play_side_sound_at(obj, obj->side, 0xd);
    }
    if ((u8)obj->step_flags == 0) {
        move_x_by_facing_with_decel_slot04_03(obj);
    }
}
