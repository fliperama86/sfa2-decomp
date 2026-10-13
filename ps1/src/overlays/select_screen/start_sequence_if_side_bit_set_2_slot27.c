/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern SequenceStep *data_80027bc4_slot27[];
extern SequenceStep *data_80027c18_slot27[];
extern void (*data_80026ee8_slot27[])(Object *);

void start_sequence_if_side_bit_set_2_slot27(Object *obj) {
    u8 *mode = &game_state.mode;
    int *side = scratch_word_b;
    Object *o = obj->other;

    if ((((*mode | game_state.side_mask) >> (*side = o->side)) & 1) != 0) {
        obj->state_1 = 0;
        obj->state_0 = obj->state_0 + 1;
        set_sequence_from_table_5(obj, obj->variant_index, data_80027bc4_slot27);
        obj->field_01 = 1;
        if (((*mode >> *side) & 1) != 0) {
            obj->state_1 = obj->state_1 + 1;
            set_sequence_from_table_5(obj, obj->variant_index, data_80027c18_slot27);
        }
    }
}

void call_state_1_handler_from_slot27_table_a_5_slot27(Object *object) {
    data_80026ee8_slot27[object->state_1](object);
}
