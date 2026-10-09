/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801c45d8_slot04_04[];
void func_801b49d0_slot04_04(Object *obj);

void func_801b4878_slot04_04(Object *obj) {
    int t = 0xc;

    obj->field_07 = 3;
    obj->field_128 = 4;
    obj->field_159 = 1;
    if (obj->field_129 == 0) {
        if (obj->field_12a != 0) {
            if (obj->field_218 != 0 && obj->field_70 - 0x30 < obj->pos_y) {
                if (func_8013ffe4(obj, -0x20, 0x20, 0, 0x10) & 0xff) {
                    obj->field_04 = 1;
                    obj->field_05 = 2;
                    obj->field_06 = 0;
                    obj->field_07 = 0;
                    return;
                }
            }
        }
    } else if (obj->field_12a == 2 && obj->field_219 != 0) {
        func_801b49d0_slot04_04(obj);
        return;
    }
    func_80141f28(obj, data_801c45d8_slot04_04[obj->field_12a >> 1]);
    if (obj->field_48 != 0) {
        t = 0x12;
    }
    if (obj->field_129 != 0) {
        t += 3;
    }
    func_801307e0(obj, (u16)((obj->field_12a >> 1) + t));
}
