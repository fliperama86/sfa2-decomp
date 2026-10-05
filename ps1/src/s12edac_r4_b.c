/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80130678(Object *object, int arg);

void func_8012f8c4(Object *object) {
    int arg;

    if (game_state.field_4d != 0) {
        func_8012ff80(object);
    } else if (func_8012f56c(object)) {
        func_8012f59c(object);
    } else {
        object->field_04 = 1;
        object->field_05 = 0;
        object->field_06 = 4;
        object->field_07 = 0;
        object->field_0b = object->field_158;
        if (object->field_157 == 0) {
            arg = 10;
        } else {
            object->field_04 = 1;
            object->field_05 = 0;
            object->field_06 = 4;
            object->field_07 = 1;
            arg = 13;
        }
        func_80130678(object, arg);
    }
}

unsigned char func_8012f970(Object *object) {
    data_80186000 = 0;
    if (object->field_7e == 0) {
        func_8012f9b0(object);
        return data_80186000;
    }
    return 0;
}
