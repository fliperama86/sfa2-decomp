/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_rise_until_apex_then_set_sequence_slot04_02(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    u16 a;

    *(s32 *)&o->pos_x_frac += o->vel_x;
    *(s32 *)&o->pos_y_frac -= o->vel_y;
    o->vel_y += o->accel_y;
    if (o->vel_y < 0) {
        o->state_3++;
        play_sound_at(o, o->side, 0x320);
        if (o->field_49 != 0) {
            a = 0x70;
        } else {
            a = 0x20;
        }
        if (o->direction_index != 0) {
            obj->step_count = o->direction_index;
        } else {
            obj->step_count = 1;
        }
        a += o->direction_index >> 1;
        set_sequence_by_side_and_mode(o, a);
    } else {
        update_sequence(o);
    }
}
