/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b3048_slot04_05(Object *object);

void func_801b2bac_slot04_05(Object *obj) {
    s16 t = obj->step_flags;
    if (t & 0x8000) {
        obj->other->field_249 = 5;
        func_801312b8(obj);
    } else {
        switch ((u8)t) {
        case 0:
            break;
        default:
            if (obj->vel_x >= 0) {
                func_801b3048_slot04_05(obj);
            }
            break;
        case 3:
            obj->vel_x = 0x40000;
            obj->accel_x = 0xffff0000;
            break;
        }
        update_sequence(obj);
    }
}
