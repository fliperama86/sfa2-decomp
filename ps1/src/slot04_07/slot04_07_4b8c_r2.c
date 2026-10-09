/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b41c4_slot04_07(Object *obj);

void func_801b4cec_slot04_07(Object *obj) {
    Slot04aObj *o = (Slot04aObj *)obj;

    if (func_801b41c4_slot04_07(obj) >= 0 || obj->field_70 > obj->pos_y) {
        func_80130efc(obj);
    } else {
        obj->field_07 = 1;
        obj->field_14 = 0;
        obj->field_45 = 0;
        obj->pos_y = o->field_70;
        func_80120554(obj, obj->side, 0x314);
        func_801307e0(obj, 0x2e);
    }
}

void func_801b4d74_slot04_07(Object *obj) {
    int v;

    obj->field_07 = 3;
    obj->field_159 = 1;
    if (obj->field_218 != 0 && obj->field_70 - obj->pos_y >= 0x30 &&
        (u8)func_8013ffe4(obj, -0x20, 0x20, 0, 0x20) != 0) {
        obj->field_05 = 2;
        obj->field_04 = 1;
        obj->field_06 = 0;
        obj->field_07 = 0;
        obj->field_128 = 4;
        return;
    }
    func_80141f28(obj, obj->field_12a >> 1);
    if (obj->field_219 != 0) {
        obj->field_4c = obj->field_4c >> 1;
        func_801307e0(obj, 0x1f);
    } else {
        v = 0xc;
        if (obj->field_48 != 0) {
            v = 0x12;
        }
        if (obj->field_129 != 0) {
            v += 3;
        }
        func_801307e0(obj, (s16)((obj->field_12a >> 1) + v));
    }
}
