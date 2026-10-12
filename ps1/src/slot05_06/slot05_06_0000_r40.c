/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801cc814_slot05_06(Object *obj);

void func_801cb2e4_slot05_06(Object *obj) {
    if (obj->vel_x >= 0) {
        Object *other;
        func_801cc814_slot05_06(obj);
        if (obj->field_67 != 0) {
            other = obj->other;
            obj->field_67 = 0;
            if (other->field_45 == 0 && other->field_61 != 0xff) {
                if ((s16)other->health >= 0) {
                    obj->state_3 += 4;
                } else {
                    obj->accel_x = -0x6000;
                    obj->state_3++;
                    update_sequence(obj);
                }
            }
        }
        update_sequence(obj);
    } else {
        obj->accel_x = -0x6000;
        obj->state_3++;
        update_sequence(obj);
    }
}
