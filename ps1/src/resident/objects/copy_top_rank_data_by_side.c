/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void copy_top_rank_data_by_side(Object *dest, Object *object) {
    int i;

    if (object->score_rank == 1) {
        if (object->side == 0) {
            dest->compare_limit = *(u16 *)&dest->list_offset;
            for (i = 0; i < 0x40; i++) {
                ((u8 *)dest + i)[0x15e] = ((u8 *)dest + i)[0x19e];
            }
        } else {
            dest->compare_limit = dest->saved_script_word;
            for (i = 0; i < 0x40; i++) {
                ((u8 *)dest + i)[0x15e] = ((u8 *)dest + i)[0x1de];
            }
        }
    }
}

void call_table_function_by_state_0_12(Block172 *block) {
    Object *object = (Object *)block;
    table_8018191c[object->state_0](object);
}

void init_object_depth_and_advance_state_0_2(Object *object) {
    object->ot_depth = 1;
    object->field_0c = 0xff;
    object->field_01 = 0;
    object->state_0++;
    clear_object_side_table_entries(object);
}
