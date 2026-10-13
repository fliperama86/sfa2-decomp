/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


int call_slot_stage_handler_in_fourth_table(Object *object, int index, u8 arg) {
    table_8017ac08[object->slots[(u8)index].stage](object, (u8)index, arg);
    return slot_result_flag;
}
