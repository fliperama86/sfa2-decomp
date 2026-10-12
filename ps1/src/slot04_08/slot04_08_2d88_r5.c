/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void push_other_x_if_hit_check_slot04_08(Object *obj);
extern u16 data_801c3e98_slot04_08;

void func_801b3260_slot04_08(Object *obj) {
    int x;

    if ((s16)obj->step_flags & 0x8000) {
        obj->vel_x = 0x80000;
        obj->state_3++;
        obj->accel_x = -0xc000;
        x = 0x160;
        obj->active_flag = 0;
        if (obj->facing_side == 0) {
            x = -0x160;
            obj->vel_x = -obj->vel_x;
            obj->accel_x = -obj->accel_x;
        }
        obj->countdown = 0;
        data_801c3e98_slot04_08 = x + obj->pos_x;
        push_other_x_if_hit_check_slot04_08(obj);
    }
    update_sequence(obj);
}

void func_801b3304_slot04_08(Object *obj) {
    s16 d;
    u16 lim = 0xa0;
    Object *other;

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
    set_sequence_by_side_and_mode(obj, 0x54);
}
