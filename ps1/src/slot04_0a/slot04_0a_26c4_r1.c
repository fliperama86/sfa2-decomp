/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c0690_slot04_0a[];
void func_801b3e68_slot04_0a(Object *obj);

void func_801b26c4_slot04_0a(Object *obj) {
    int t;
    update_sequence(obj);
    t = (s16)obj->step_flags;
    if (t & 0x80) {
        obj->state_3++;
        func_801204f4(obj, obj->side, 0xd);
        obj->field_45 = 1;
        obj->field_67 = 0;
        obj->accel_x = 0;
        obj->vel_y = 0;
        obj->accel_y = 0;
        t = 0x90000;
        if (obj->field_0b == 0) {
            t = 0xfff70000;
        }
        obj->vel_x = t;
        func_801b3e68_slot04_0a(obj);
        func_801307e0(obj, (obj->field_12a >> 1) + 0x3c);
    } else if (t == 2) {
        obj->field_165 = 0;
        t = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
            t = (obj->field_12a >> 1) + 1;
        }
        obj->field_27b = data_801c0690_slot04_0a[t];
        func_801204f4(obj, obj->side, 5);
    } else if (t == 1) {
        if (obj->field_4b == 0) {
            t = -1;
        }
        obj->field_165 = t;
        obj->step_flags = obj->step_flags & 0xff00;
        func_801483a4(obj, 0x2f, 0x41);
    }
}
