/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014a364(Object *object) {
    if (object->field_207 == 0) {
        object->field_207++;
        object->script_ptr = object->field_228;
        func_8014a4f4(object);
        func_8014ccb0(object);
    } else if (object->field_207 == 1) {
        func_8014ccb0(object);
    }
}

void func_8014a3c8(Object *object) {
    if (object->field_207 == 0) {
        object->field_207++;
        object->script_ptr = object->script_ptr_1;
        func_8014a4f4(object);
        func_8014ccb0(object);
    } else if (object->field_207 == 1) {
        func_8014ccb0(object);
    }
}

void func_8014a42c(Object *object) {
    if (object->field_207 == 0) {
        object->field_207++;
        object->script_ptr = object->script_ptr_2;
        func_8014a4f4(object);
        func_8014ccb0(object);
    } else if (object->field_207 == 1) {
        func_8014ccb0(object);
    }
}

void func_8014a490(Object *object) {
    if (object->field_207 == 0) {
        object->field_207++;
        object->script_ptr = object->script_ptr_3;
        func_8014a4f4(object);
        func_8014ccb0(object);
    } else if (object->field_207 == 1) {
        func_8014ccb0(object);
    }
}
