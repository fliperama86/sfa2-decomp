/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801bf09c_slot04_01[];
extern s32 data_801bf0a0_slot04_01[];
extern u8 data_801bf0b4_slot04_01[];

void func_801b24c8_slot04_01(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    s32 unused[2];
    int i = 0;
    u16 t;

    update_sequence(obj);
    t = obj->step_flags;
    if (t & 0x80) {
        obj->accel_x = -0x8000;
        obj->accel_y = -0x6000;
        obj->other_state_code = 2;
        obj->state_3++;
        obj->vel_x = data_801bf09c_slot04_01[obj->direction_index];
        obj->vel_y = data_801bf0a0_slot04_01[obj->direction_index];
    } else {
        if ((t & 0xff) == 2) {
            obj->step_flags = t & 0xff00;
            obj->vel_x = 0x80000;
            obj->accel_x = -0x8000;
        }
        if (*(u8 *)&obj->step_flags == 0) {
            if (obj->field_165 != 0) {
                obj->field_165 = 0;
                if (obj->field_4b == 0) {
                    obj->other->field_6b = 0xa;
                    i = (obj->direction_index >> 1) + 1;
                }
                obj->other_lock = data_801bf0b4_slot04_01[i];
                play_side_sound_at(obj, obj->side, 4);
            }
            if (obj->facing_side == 0) {
                *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
            } else {
                *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
            }
            obj->vel_x = obj->vel_x + obj->accel_x;
            if (obj->vel_x < 0) {
                obj->vel_x = 0;
                obj->accel_x = 0;
            }
        }
        obj->other_state_code = 2;
    }
}
