/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c2808_slot04_0d[];
extern u8 data_801c2820_slot04_0d[];

void func_801b2794_slot04_0d(Object *obj) {
    int i;
    u8 t;
    u16 a;
    s32 *tbl = data_801c2808_slot04_0d;

    t = obj->step_flags;
    obj->other_state_code = 2;
    update_sequence(obj);
    if ((t & 0x80) != 0) {
        i = obj->direction_index;
        obj->accel_x = -0x8000;
        obj->accel_y = 0x6000;
        obj->state_3++;
        obj->vel_x = tbl[i];
        obj->vel_y = -tbl[i + 1];
    } else {
        a = obj->step_flags;
        i = a & 0xff;
        if (i == 2) {
            obj->step_flags = a & 0xff00;
            obj->vel_x = 0x80000;
            obj->accel_x = -0x8000;
        }
        if ((u8)obj->step_flags == 0) {
            i = 0;
            if (obj->field_165 != 0) {
                obj->field_165 = 0;
                if (obj->field_4b == 0) {
                    obj->other->field_6b = 0xa;
                    i = obj->direction_index >> 1;
                    i += 1;
                }
                ((Slot04bObj *)obj)->table_value = data_801c2820_slot04_0d[i];
            }
            i = obj->vel_x;
            if (obj->facing_side == 0) {
                i = -i;
            }
            *(s32 *)&obj->pos_x_frac = i + *(s32 *)&obj->pos_x_frac;
            obj->vel_x = obj->vel_x + obj->accel_x;
            if (obj->vel_x < 0) {
                obj->vel_x = 0;
                obj->accel_x = 0;
            }
        }
    }
}
