/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801c3e98_slot04_08;

void move_x_to_target_then_set_sequence_by_direction_slot04_08(Object *obj) {
    s16 d;
    u16 lim = 0xa0;
    Object *other;
    int k;

    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    d = (u16)obj->pos_x - data_801c3e98_slot04_08;
    if (obj->facing_side != 0) {
        d = -d;
    }
    if (d >= 0) {
        d = 2;
        if (obj->vel_x < 0) {
            d = 1;
        }
        if (obj->bound_push_flag != d) {
            other = obj->other;
            d = -0x20;
            if (obj->facing_side != 0) {
                d = 0x20;
            }
            d = d + (u16)obj->pos_x - other->pos_x + 0x50;
            if (lim < (u16)d) {
                update_sequence(obj);
                return;
            }
        }
    }
    obj->state_3++;
    k = 0x52;
    if (obj->direction_index != 4) {
        k = 0x51;
    }
    set_sequence_by_side_and_mode(obj, k);
}

void move_x_with_accel_until_sequence_ends_slot04_08(Object *obj) {
    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    obj->vel_x = obj->vel_x + obj->accel_x;
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_3++;
        set_sequence_by_side_and_mode(obj, 0x53);
    } else {
        update_sequence(obj);
    }
}
