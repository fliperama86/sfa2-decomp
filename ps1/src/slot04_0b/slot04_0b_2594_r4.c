/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801b2510_slot04_0b(Object *object);

void func_801b2a1c_slot04_0b(Object *obj) {
    if (func_801b2510_slot04_0b(obj)) {
        func_80130efc(obj);
    } else {
        if (obj->field_12a == 4 && obj->field_a0 == 0) {
            obj->field_07 = 6;
            obj->field_a0 = 3;
            func_801307e0(obj, 0x48);
        } else {
            obj->field_07++;
            obj->field_45 = 0;
            obj->pos_y = ((Slot04aObj *)obj)->field_70;
            func_801209c4(obj);
            func_801307e0(obj, 0x2a);
        }
    }
}
