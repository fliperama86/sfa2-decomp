/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c06ac_slot04_0a[];
void func_801b3e00_slot04_0a(Object *obj);

void func_801b2a38_slot04_0a(Object *obj) {
    int k;
    int b;
    u8 f;
    update_sequence(obj);
    k = -1;
    if (obj->field_4b != 0) {
        k = 1;
    }
    obj->field_165 = k;
    f = ((Slot04aObj *)obj)->field_3a;
    if (f == 0) {
        obj->field_165 = 0;
        if (((s16)obj->step_flags & 0xff00) != 0x100) {
            func_801b3e00_slot04_0a(obj);
            if (!(obj->pos_y < obj->ground_y)) {
                k = 0xa0000;
                b = -0x1000;
                obj->vel_y = 0x28000;
                obj->accel_y = -0x3000;
                obj->state_3++;
                if (obj->field_0b == 0) {
                    k = 0xfff60000;
                    b = 0x1000;
                }
                obj->vel_x = k;
                obj->accel_x = b;
                func_801307e0(obj, (obj->field_12a >> 1) + 0x33);
            }
        }
    } else if (f == 2) {
        k = 0;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
            k = (obj->field_12a >> 1) + 1;
        }
        obj->field_27b = data_801c06ac_slot04_0a[k];
    }
}
