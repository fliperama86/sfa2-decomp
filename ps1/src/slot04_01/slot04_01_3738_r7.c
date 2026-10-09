/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4218_slot04_01(Object *obj) {
    int d = 0x18;
    obj->field_07++;
    obj->field_0b = obj->field_158;
    if (obj->field_12a != 0 && obj->field_218 != 0) {
        if ((u8)func_8013f8c4(obj, -0x14, 0x14) != 0) {
            obj->field_04 = 1;
            obj->field_05 = 2;
            obj->field_06 = 0;
            obj->field_07 = 0;
            return;
        }
        if (obj->field_12a == 2 && obj->field_219 != 0) {
            obj->field_159 = 1;
            obj->field_07 = 2;
            func_80141f28(obj, 1);
            func_801307e0(obj, 0x1c);
            return;
        }
        if (obj->field_0b == 0) {
            obj->pos_x = obj->pos_x - d;
        } else {
            obj->pos_x = d + obj->pos_x;
        }
    }
    obj->field_159 = 1;
    func_80130dc0(obj);
}
