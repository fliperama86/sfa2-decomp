/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_80130184(Object *object);

void func_8012d910(Object *object) {
    if ((u8)func_80130184(object) != 0) {
        func_80130efc(object);
    } else {
        object->field_07 = object->field_07 + 1;
        func_80131d68(object);
        object->field_45 = 0;
        object->field_46 = 8;
        object->pos_y = object->field_70;
        func_801308c4(object, 0x16);
        if ((s16)object->field_5c >= 0) {
            func_80120a30(object, 1, 0);
        } else {
            func_80120a30(object, 1, 1);
        }
    }
}

void func_8012d9b4(Object *object) {
    object->field_46 = (s16)object->field_46 - 1;
    if ((s16)object->field_46 != 0) {
        func_80130efc(object);
    } else if ((s16)object->field_5c < 0) {
        func_8012f4dc(object);
    } else {
        object->field_07 = object->field_07 + 1;
        func_801308c4(object, 0x19);
    }
}
