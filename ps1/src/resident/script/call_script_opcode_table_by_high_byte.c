/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void call_script_opcode_table_by_high_byte(Object *object) {
    data_8017cf9c[object->script_opcode_high >> 1](object);
}

void call_script_arg_table_by_script_arg(Object *object) {
    data_8017cfc4[object->script_arg >> 1](object);
}
