/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void call_table_function_by_state_0_2(Block172 *block) {
    Object *object = (Object *)block;
    table_8017c978[object->state_0](object);
}

void start_sequence_by_group_and_advance_state_0(Object *object) {
    object->field_01 = 0;
    object->state_0++;
    if (object->field_0e == 4) {
        set_sequence_from_table(object, table_8017c90c, object->table_index);
    } else if (object->field_0e == 8) {
        set_sequence_from_table_2(object, table_8017c910, object->table_index);
    } else if (object->field_0e == 0xc) {
        set_sequence_from_table_3(object, table_8017c960, object->table_index);
    }
}
