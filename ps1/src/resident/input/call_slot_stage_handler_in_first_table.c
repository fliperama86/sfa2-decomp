/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


u8 call_slot_stage_handler_in_first_table(Object *object, u8 index, u8 arg) {
    table_8017abb8[object->slots[index].stage](object, index, arg);
    return slot_result_flag;
}
