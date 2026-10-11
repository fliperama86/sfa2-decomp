/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b27a4_slot04_06(Object *obj) {
    Slot04aObj *s = (Slot04aObj *)obj;
    s32 a;

    s->field_1c2 = s->field_1c2 - 1;
    if (s->field_1c2 & 0x80) {
        obj->state_3 = obj->state_3 + 1;
        func_801204f4(obj, obj->side, 5);
        obj->vel_y = -0x100000;
        obj->vel_x = 0;
        obj->accel_x = 0;
        func_801307e0(obj, 0x24);
    } else {
        a = 0x4000;
        if (obj->field_0b == 0) {
            a = -0x4000;
        }
        *(s32 *)&obj->pos_x_frac += a;
        *(s32 *)&obj->pos_y_frac += -0x4000;
        update_sequence(obj);
    }
}
