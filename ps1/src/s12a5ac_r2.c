/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_8012a8dc(Object *object) {
    handler_table_19b0[object->field_07](object);
}

void func_8012a91c(Object *object) {
    int index;
    if ((s16)object->field_3a >= 0) {
        func_80130efc(object);
        return;
    }
    object->field_07 = object->field_07 + 1;
    object->field_157 = 0;
    object->field_45 = 1;
    if (object->field_48 == 1) {
        index = 0x13;
    } else if (object->field_48 & 0x80) {
        index = 0x14;
    } else {
        index = 0x12;
    }
    func_80130678(object, index);
}
