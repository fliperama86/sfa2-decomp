/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130dc0(Object *object);

void func_801b0d24_slot04_07(Object *obj) {
    u8 one;

    obj->field_07++;
    one = 1;
    obj->field_159 = one;
    ((Slot04aObj *)obj)->field_1ca = 0;
    if (obj->field_12a != 4 || obj->field_25f != 0) {
        func_80130dc0(obj);
    } else if ((obj->field_130 & 0x8000) == 0) {
        obj->field_07 = 2;
        func_80120554(obj, obj->side, 0x324);
        obj->field_4c = 0x90000;
        obj->field_54 = -0x8000;
        func_80130dc0(obj);
        obj->field_278 = one;
    } else {
        obj->field_07 = 3;
        func_80141f28(obj, 1);
        func_80130ec0(obj);
        func_801307e0(obj, 0x2d);
    }
}
