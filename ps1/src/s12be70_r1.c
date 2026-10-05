/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



int func_8012be70(Object *object) {
    if (object->field_cd == 0) {
        if ((s16)object->field_134 & 0xf000) return 1;
    } else {
        func_8012bf4c(object);
    }
    return 0;
}

int func_8012bec0(Object *object) {
    int result;
    if (object->field_cd == 0) {
        result = *(u8 *)&object->field_132 == 0;
    } else {
        func_8012bf4c(object);
        result = 0;
    }
    return result;
}

int func_8012bf00(Object *object) {
    int result;
    if (object->field_cd == 0) {
        result = ((object->field_134 >> 8) & 0xfc) != 0;
    } else {
        func_8012bf4c(object);
        result = 0;
    }
    return result;
}

void func_8012bf4c(Object *object) {
    func_8014a170(object, data_8017d818);
}
