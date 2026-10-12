/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_8012d264(Object *object) {
    int index;
    if (object->pressed_bits == 0) {
        index = 0xc;
    } else {
        object->field_157 = object->field_157 + 1;
        object->action_timer = 0;
        index = 0xe;
    }
    func_801308c4(object, index);
}

void func_8012d2ac(Object *object) {
    if ((s16)object->meter >= 0x30 && (s16)object->health >= 0 && object->field_221 != 0) {
        func_801303a0(object);
    } else {
        func_8012d310(object);
    }
}
