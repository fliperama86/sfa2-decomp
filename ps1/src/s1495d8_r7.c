/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014a5c0(Object *object) {
    data_8017cf9c[object->script_opcode_high >> 1](object);
}

void func_8014a604(Object *object) {
    data_8017cfc4[object->script_arg >> 1](object);
}
