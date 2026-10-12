/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u16 *data_801e52c8_slot2a;
extern u8 data_801e25d8_slot2a[];
extern SequenceStep *data_801e21f4_slot2a[];
extern u32 data_801e21fc_slot2a[];

void check_input_advance_state_or_reset_slot2a(Object *obj) {
    SlotInput *in;
    int k;

    if (game_state.status_flags != 0) {
        in = (SlotInput *)data_801e52c8_slot2a;
        if (in->buttons & 0x8000) {
            obj->state_0++;
        } else if ((k = obj->table_index) == (in->top_bit_word >> 15)) {
            if (data_801e25d8_slot2a[k] == 0) {
                obj->state_1 = 0;
            }
        }
    }
}

void release_object_record_slot2a(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void set_sequence_data_ptr_and_sequence_slot2a(Object *obj, u8 idx) {
    data_801e21f4_slot2a[obj->table_index]->data_ptr = data_801e21fc_slot2a[idx];
    set_sequence_from_table_5(obj, obj->table_index, data_801e21f4_slot2a);
}
