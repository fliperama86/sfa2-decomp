/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b223c_slot04_03(Object *obj);

void func_801b1fd0_slot04_03(Object *obj) {
    s16 t;

    if (obj->field_12a >= 3) {
        update_sequence(obj);
        t = obj->step_flags;
        if (t & 0x8000) {
            if (obj->field_12a != 0) {
                obj->state_3++;
                play_side_sound_at(obj, obj->side, 5);
                play_side_sound_at(obj, obj->side, 0xd);
                obj->vel_x = 0xc0000;
                obj->accel_x = -0x8000;
                obj->vel_y = -0x90000;
                obj->accel_y = 0x6000;
                func_801307e0(obj, 0x38);
            } else {
                obj->state_3 = 6;
                func_801307e0(obj, 0x28);
            }
        } else if ((u8)t == 0) {
            func_801b223c_slot04_03(obj);
        }
    } else if ((u8)obj->step_flags == 0) {
        obj->field_45 = 1;
        func_801b223c_slot04_03(obj);
        *(s32 *)&obj->pos_y_frac += obj->vel_y;
        obj->vel_y += obj->accel_y;
        if (obj->ground_y < obj->pos_y) {
            obj->pos_y = obj->ground_y;
            obj->field_45 = 0;
            func_801209c4(obj);
            obj->state_3 = 6;
            func_801307e0(obj, 0x28);
        } else {
            update_sequence(obj);
        }
    } else {
        update_sequence(obj);
    }
}
