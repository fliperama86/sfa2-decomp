/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b324c_slot04_09(Object *obj) {
    if (((s16)obj->step_flags & 0x8000) != 0) {
        obj->field_45 = 1;
        obj->accel_y = -0x6000;
        obj->state_3++;
        obj->vel_y = 0x88000;
        if (obj->field_a3 == 0) {
            if (obj->field_0b != 0) {
                obj->vel_x = 0xc0000;
                obj->accel_x = -0x8000;
            } else {
                obj->vel_x = 0xfff40000;
                obj->accel_x = 0x8000;
            }
            func_801307e0(obj, 0x33);
        } else {
            obj->vel_x = ((obj->other->pos_x - obj->pos_x) >> 4) << 16;
            obj->accel_x = 0;
            func_801307e0(obj, 0x33);
        }
    } else {
        if (obj->field_67 != 0 && obj->other->field_61 != 0xff) {
            obj->field_a3 = 0x80;
        }
        update_sequence(obj);
    }
}
