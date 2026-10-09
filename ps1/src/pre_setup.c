/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

Dir func_8013054c(Object *object);

u16 func_80130470(Object *object) {
    return (u8)object->field_134;
}


void func_8013047c(Object *object) {
    object->field_04 = 1;
    object->field_05 = 0;
    object->field_06 = 5;
    object->field_07 = 0;
    if (object->field_7e == 0) object->field_0b = object->field_158;
    func_80142c04(object);
    object->field_67 = 0;
    func_80130504(object);
    object->field_157 = ((u8)func_8012f618(object) != 0) << 1;
    object->field_128 = object->field_157;
    func_8012b45c(object);
}

void func_80130504(Object *object) {
    Dir d = func_8013054c(object);
    object->field_12a = d.first;
    object->field_129 = d.second;
}

Dir func_8013054c(Object *object) {
    Dir d;
    u16 pad = object->field_134;
    d.second = 0;
    d.first = 0;
    if (pad & 0x80) {
        d.first = 0;
        d.second = 0;
    } else if (pad & 0x10) {
        d.first = 2;
        d.second = 0;
    } else if (pad & 0x4) {
        d.first = 4;
        d.second = 0;
    } else if (pad & 0x40) {
        d.first = 0;
        d.second = 2;
    } else if (pad & 0x20) {
        d.first = 2;
        d.second = 2;
    } else if (pad & 0x8) {
        d.first = 4;
        d.second = 2;
    } else if (pad & 0x1) {
        d.first = 0;
        d.second = 0;
    } else if (pad & 0x2) {
        d.first = 0;
        d.second = 2;
    }
    return d;
}
