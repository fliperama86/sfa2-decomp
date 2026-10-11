/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c6380_slot04_02[];
extern s32 data_801c6384_slot04_02[];
extern u8 data_801c6398_slot04_02[];

void func_801b2e38_slot04_02(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    s32 unused[2];
    u8 i;
    u16 t;

    obj->field_249 = 2;
    update_sequence(obj);
    t = obj->step_flags;
    if (t & 0x80) {
        obj->accel_x = -0x8000;
        obj->accel_y = -0x6000;
        obj->state_3++;
        obj->vel_x = data_801c6380_slot04_02[obj->field_12a];
        obj->vel_y = data_801c6384_slot04_02[obj->field_12a];
    } else {
        if ((t & 0xff) == 2) {
            obj->step_flags = t & 0xff00;
            obj->vel_x = 0x80000;
            obj->accel_x = -0x8000;
        }
        if (*(u8 *)&obj->step_flags == 0) {
            if (obj->field_165 != 0) {
                i = 0;
                obj->field_165 = 0;
                if (obj->field_4b == 0) {
                    obj->other->field_6b = 0xa;
                    i = (obj->field_12a >> 1) + 1;
                }
                obj->field_27b = data_801c6398_slot04_02[i];
            } else {
                if (obj->field_0b != 0) {
                    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
                } else {
                    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
                }
                obj->vel_x = obj->vel_x + obj->accel_x;
                if (obj->vel_x < 0) {
                    obj->accel_x = 0;
                    obj->vel_x = 0;
                }
            }
        }
    }
}
