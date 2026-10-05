/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_8012aef8(Object *object) {
    if (func_80149c30(object)) {
        func_80130304(object);
    } else {
        if (!func_80149d48(object)) {
            object->field_07 = 1;
        }
        func_80130efc(object);
    }
}
