/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_8012d030(Object *object) {
    if ((s16)object->field_5c < 0) {
        func_8012f4dc(object);
    } else {
        object->state_0 = 1;
        object->state_1 = 1;
        object->state_2 = 2;
        object->state_3 = 0;
        object->field_15b = 0;
        object->field_247 = 0;
    }
}

void func_8012d084(Object *object) {
    if (func_8012eff8(object) != 0) {
        object->state_3 = 2;
    }
}

void func_8012d0bc(Object *object) {
    play_side_sound_at(object, object->side, 0);
}

void func_8012d0e0(Object *object) {
    play_side_sound_at(object, object->side, 1);
}

void func_8012d104(Object *object) {
    table_80171a30[object->state_3](object);
}

void func_8012d144(Object *object) {
    object->state_3 = object->state_3 + 1;
    func_8012d170(object);
}
