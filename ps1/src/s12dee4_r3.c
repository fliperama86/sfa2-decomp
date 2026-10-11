/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8012e334(Object *object) {
    if ((short)object->field_5c < 0) {
        func_8012f4dc(object);
    } else {
        object->state_0 = 1;
        object->state_1 = 1;
        object->state_2 = 2;
        object->state_3 = 0;
        object->field_15b = 0;
        object->field_247 = 0;
        object->field_29f = 0;
    }
}
