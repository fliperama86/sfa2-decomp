/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8014aa4c(Object *object) {
    if ((*(unsigned *)&object->field_208 & 0xffff0000) == 0) {
        func_8014ab74(object);
    } else {
        func_8014dc30(object);
        if (object->field_21c == 0) {
            func_8014c914(object);
        } else {
            object->field_208 = 0;
            object->field_209 = 8;
            func_8014ae54(object);
        }
    }
}

void func_8014aacc(Object *object) {
    if (object->field_240 == 0 || object->field_14c == 0) {
        func_8014c914(object);
    } else {
        object->field_208 = 0;
        object->field_209 = 9;
        func_8014ae54(object);
    }
}

void func_8014ab20(Object *object) {
    if ((u8)func_8014e758(object)) {
        func_8014c914(object);
    } else {
        object->field_208 = 0;
        object->field_209 = 10;
        func_8014ae54(object);
    }
}

void func_8014ab74(Object *object) {
    func_8014dc30(object);
    if (object->field_21c == 0) {
        func_8014c914(object);
    } else {
        object->field_208 = 0;
        object->field_209 = 0;
        func_8014ae54(object);
    }
}

void func_8014abc8(Object *object) {
    object->field_21c = 1;
    object->field_208 = 0;
    object->field_209 = 8;
    func_8014ae54(object);
}

void func_8014abf8(Object *object) {
    u16 value = *data_80189460++;
    object->field_208 = 0;
    object->field_209 = 8;
    object->field_21c = value;
    func_8014ae54(object);
}

void func_8014ac3c(Object *object) {
    func_8014e7ec(object);
    object->field_21c = *data_80189460++;
    data_80189464 = data_80189464 - object->field_21e;
    if ((s16)data_80189464 >= 0) {
        func_8014c914(object);
    } else {
        object->field_209 = 0xb;
        object->field_208 = 0;
        object->field_04 = 1;
        object->field_05 = 0;
        object->field_06 = 0;
        object->field_07 = 0;
    }
}

void func_8014acd0(Object *object) {
    func_8014e7ec(object);
    object->field_21c = *data_80189460++;
    data_80189464 = data_80189464 - object->field_21e;
    if ((s16)data_80189464 < 0) {
        func_8014c914(object);
    } else {
        object->field_209 = 0xc;
        object->field_208 = 0;
        object->field_04 = 1;
        object->field_05 = 0;
        object->field_06 = 0;
        object->field_07 = 0;
    }
}

void func_8014ad64(Object *object) {
    if ((u8)func_8014e718(object)) {
        func_8014c914(object);
    } else {
        object->field_208 = 0;
        object->field_209 = 0xd;
        func_8014ae54(object);
    }
}

void func_8014adb8(Object *object) {
    if (func_80142030(object)) {
        object->field_208 = 3;
        object->field_209 = 0;
        object->field_04 = 1;
        object->field_05 = 0;
        object->field_06 = 5;
        object->field_07 = 0;
        object->field_159 = 1;
        object->field_21b = 0;
        object->field_0b = object->field_158;
    } else {
        func_8014c914(object);
    }
}

void func_8014ae28(Object *object) {
    object->field_67 = 0;
    object->field_208 = 0;
    object->field_209 = 5;
    func_8014ae54(object);
}

void func_8014ae54(Object *object) {
    if ((*(unsigned *)&object->field_04 & 0xffffff) != 1) {
        object->field_04 = 1;
        object->field_05 = 0;
        object->field_06 = 0;
        object->field_07 = 0;
    }
}

void func_8014ae88(Object *object) {
    table_8017d010[object->field_20f >> 1](object);
}

void func_8014aecc(Object *object) {
    func_8014e7ec(object);
    data_80189464 = data_80189464 - object->field_21e;
    if ((s16)data_80189464 >= 0) {
        func_8014c914(object);
    } else {
        object->field_208 = 1;
        object->field_04 = 1;
        object->field_209 = 0;
        object->field_05 = 0;
        object->field_06 = 2;
        object->field_07 = 0;
        object->field_48 = 0;
        object->field_21a = 0;
    }
}

void func_8014af50(Object *object) {
    func_8014e7ec(object);
    data_80189464 = data_80189464 - object->field_21e;
    if ((s16)data_80189464 >= 0) {
        func_8014c914(object);
    } else {
        object->field_208 = 1;
        object->field_209 = 1;
        object->field_04 = 1;
        object->field_05 = 0;
        object->field_06 = 2;
        object->field_07 = 0;
        object->field_48 = 1;
        object->field_21a = 1;
    }
}
