/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b28c8_slot04_03(Object *obj) {
    s16 t = obj->field_3a;
    u8 c;

    if (t & 0x8000) {
        obj->field_07 = 0xb;
        func_801307e0(obj, 0x2f);
    } else if (t == 0) {
        c = obj->field_129 - 1;
        obj->field_129 = c;
        if (c & 0x80) {
            obj->field_07 = 0xb;
            func_801307e0(obj, 0x2f);
        } else {
            obj->field_07++;
            func_801204f4(obj, obj->side, 6);
            func_801204f4(obj, obj->side, 0xc);
            func_801307e0(obj, 0x2c);
        }
    } else {
        func_80130efc(obj);
    }
}

void func_801b298c_slot04_03(Object *obj) {
    s16 t = obj->field_3a;
    u8 c;

    if (t & 0x8000) {
        obj->field_07 = 0xb;
        func_801307e0(obj, 0x30);
    } else if (t == 0) {
        if (obj->field_12a == 0) {
            goto set;
        }
        c = obj->field_129 - 1;
        obj->field_129 = c;
        if (c & 0x80) {
        set:
            obj->field_07 = 0xb;
            func_801307e0(obj, 0x30);
        } else {
            obj->field_07++;
            func_801204f4(obj, obj->side, 6);
            func_801204f4(obj, obj->side, 0xc);
            func_801307e0(obj, 0x2d);
        }
    } else {
        func_80130efc(obj);
    }
}
