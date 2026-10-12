/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8012d910(Object *object) {
    if ((u8)func_80130184(object) != 0) {
        update_sequence(object);
    } else {
        object->state_3 = object->state_3 + 1;
        func_80131d68(object);
        object->field_45 = 0;
        object->field_46 = 8;
        object->pos_y = object->ground_y;
        func_801308c4(object, 0x16);
        if ((s16)object->health >= 0) {
            func_80120a30(object, 1, 0);
        } else {
            func_80120a30(object, 1, 1);
        }
    }
}

void func_8012d9b4(Object *object) {
    object->field_46 = (s16)object->field_46 - 1;
    if ((s16)object->field_46 != 0) {
        update_sequence(object);
    } else if ((s16)object->health < 0) {
        func_8012f4dc(object);
    } else {
        object->state_3 = object->state_3 + 1;
        func_801308c4(object, 0x19);
    }
}
