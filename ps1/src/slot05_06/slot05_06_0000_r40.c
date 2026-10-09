/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801cc814_slot05_06(Object *obj);

void func_801cb2e4_slot05_06(Object *obj) {
    if (obj->field_4c >= 0) {
        Object *other;
        func_801cc814_slot05_06(obj);
        if (obj->field_67 != 0) {
            other = obj->other;
            obj->field_67 = 0;
            if (other->field_45 == 0 && other->field_61 != 0xff) {
                if ((s16)other->field_5c >= 0) {
                    obj->field_07 += 4;
                } else {
                    obj->field_54 = -0x6000;
                    obj->field_07++;
                    func_80130efc(obj);
                }
            }
        }
        func_80130efc(obj);
    } else {
        obj->field_54 = -0x6000;
        obj->field_07++;
        func_80130efc(obj);
    }
}
