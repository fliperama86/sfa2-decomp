/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0e8c_slot04_07(Object *obj) {
    s16 t = obj->step_flags;
    u16 *q;
    FrameRecord *fr;

    if (t & 0x8000) {
        func_80131468(obj);
    } else {
        if ((t & 0xff) != 0) {
            if (obj->field_0b == 0) {
                *(s32 *)&obj->pos_x_frac -= obj->vel_x;
            } else {
                *(s32 *)&obj->pos_x_frac += obj->vel_x;
            }
            obj->vel_x += obj->accel_x;
            if (obj->vel_x < 0) {
                q = (u16 *)obj->sequence;
                q += 6;
                obj->sequence = (SequenceStep *)q;
                obj->step_timer = q[0];
                obj->step_flags = q[1];
                fr = obj->frames + obj->sequence->frame_index;
                obj->frame = fr;
                obj->field_4a = fr->field_0d;
                obj->field_80 = 1;
            }
        }
        func_80130efc(obj);
    }
}

void func_801b0f74_slot04_07(Object *obj) {
    if ((obj->step_flags & 0xff) != 0) {
        obj->vel_x = 0xfffd8000;
        obj->accel_y = -0x5000;
        obj->vel_y = 0x58000;
        obj->accel_x = 0;
        obj->state_3++;
        if (obj->field_0b == 0) {
            obj->vel_x = 0x28000;
        }
        obj->field_45 = 1;
    }
    func_80130efc(obj);
}
