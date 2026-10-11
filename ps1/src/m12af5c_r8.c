/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8012df44(Object *object) {
    if ((u8)func_80130184(object) == 0) {
        object->pos_y = (u16)object->field_70;
        object->state_0 = 1;
        object->state_2 = 3;
        object->state_1 = 0;
        object->state_3 = 2;
        object->field_159 = 0;
        object->field_45 = 0;
        func_801209c4(object);
        if (func_8012f970(object) != 0) {
            func_8012fe60(object);
        } else {
            func_80130678(object, 0x11);
        }
    } else {
        if ((s16)object->step_flags < 0) {
            object->state_0 = 1;
            object->state_1 = 0;
            object->state_2 = 3;
            object->state_3 = 1;
            object->field_159 = 0;
        }
        update_sequence(object);
    }
}
