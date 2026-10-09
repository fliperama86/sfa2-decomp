/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8012d4e4(Object *object) {
    if ((u8)func_80130184(object)) {
        func_801376b8(object);
        if (object->field_61 == 0x13) {
            ref_other.p = object->other;
            if (ref_other.p->field_7e == 0) {
                func_80130efc(object);
            } else if (object->field_50 >= 0) {
                func_80130efc(object);
            }
        } else if (object->field_61 == 0x11) {
            func_80130efc(object);
        } else if (object->field_50 < 0) {
            ref_other.p = object->other;
            if (ref_other.p->field_7e == 0) {
                if ((s16)object->field_5c < 0 || object->field_15b != 0) {
                    func_80130efc(object);
                } else {
                    object->field_07 = 6;
                    if (object->field_2a3) {
                        object->field_2a3 = 0;
                    }
                    func_801308c4(object, 0x18);
                }
            }
        }
    } else {
        object->field_07 = object->field_07 + 1;
        if ((s16)object->field_5c < 0) {
            func_80120a30(object, 0, 1);
        } else {
            func_80120a30(object, 0, 0);
        }
        object->field_45 = 0;
        object->field_46 = 4;
        object->pos_y = (u16)object->field_70;
        func_80131df8(object);
        func_8013786c(object);
        object->field_258 = 0;
        object->field_259 = 0;
        object->field_25a = 0;
        func_8012d7e4(object);
    }
}

void func_8012d680(Object *object) {
    int v = object->field_46 - 1;
    object->field_46 = v;
    if ((s16)v != 0) {
        if (object->field_61 == 0x13) {
            ref_other.p = object->other;
            if (ref_other.p->field_7e == 0) {
                func_80130efc(object);
            } else if (object->field_50 >= 0) {
                func_80130efc(object);
            }
        } else if (object->field_61 == 0x11) {
            func_80130efc(object);
        } else if (object->field_50 < 0) {
            ref_other.p = object->other;
            if (ref_other.p->field_7e == 0) {
                if ((s16)object->field_5c < 0 || object->field_15b != 0) {
                    func_80130efc(object);
                } else {
                    object->field_07 = 6;
                    if (object->field_2a3) {
                        object->field_2a3 = 0;
                    }
                    func_801308c4(object, 0x18);
                }
            }
        }
    } else {
        object->field_45 = 0xff;
        object->field_07 = object->field_07 + 1;
        func_8012ef24(object);
        func_801308c4(object, 0x15);
    }
}
