/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/*
 * What it does (inferred, not an original name): a per-frame update of a
 * character state that moves the object along field_14 with a velocity
 * (field_50) and an acceleration (field_58). When the low byte of field_3a is
 * 0 it advances the position, and if field_50 is then negative it counts up
 * field_07, stores field_12a + 1 in field_1a4 and starts a sequence. In every
 * other case it hands over to func_80130efc.
 */
void func_801b2560_slot04_00(Object *obj) {
    if ((u8)obj->step_flags == 0) {
        u8 count;
        u16 seq;
        obj->field_45 = 1;
        /* The word at field_14 is a 32-bit value (inferred); the plain
           member access differs in 3 instruction lines. */
        *(s32 *)&obj->pos_y_frac -= obj->vel_y;
        obj->vel_y += obj->accel_y;
        if (obj->vel_y < 0) {
            count = obj->state_3 + 1;
            seq = (obj->field_12a >> 1) + 0x32;
            obj->state_3 = count;
            ((Slot04aObj *)obj)->field_1a4 = obj->field_12a + 1;
            func_801307e0(obj, seq);
            return;
        }
    }
    func_80130efc(obj);
}
