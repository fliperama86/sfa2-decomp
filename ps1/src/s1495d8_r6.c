/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void start_or_run_script_mode_0(Object *object) {
    if (object->field_207 == 0) {
        object->field_207++;
        object->script_ptr = object->script_pointer;
        read_script_word_and_run_opcode(object);
        func_8014ccb0(object);
    } else if (object->field_207 == 1) {
        func_8014ccb0(object);
    }
}

void start_or_run_script_mode_1(Object *object) {
    if (object->field_207 == 0) {
        object->field_207++;
        object->script_ptr = object->script_ptr_1;
        read_script_word_and_run_opcode(object);
        func_8014ccb0(object);
    } else if (object->field_207 == 1) {
        func_8014ccb0(object);
    }
}

void start_or_run_script_mode_2(Object *object) {
    if (object->field_207 == 0) {
        object->field_207++;
        object->script_ptr = object->script_ptr_2;
        read_script_word_and_run_opcode(object);
        func_8014ccb0(object);
    } else if (object->field_207 == 1) {
        func_8014ccb0(object);
    }
}

void start_or_run_script_mode_3(Object *object) {
    if (object->field_207 == 0) {
        object->field_207++;
        object->script_ptr = object->script_ptr_3;
        read_script_word_and_run_opcode(object);
        func_8014ccb0(object);
    } else if (object->field_207 == 1) {
        func_8014ccb0(object);
    }
}
