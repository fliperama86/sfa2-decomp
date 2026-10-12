/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void init_select_record_from_object_2_slot04_sel(Object *obj, Slot04SelRec *rec) {
    rec->state = 0;
    rec->kind = obj->kind;
    rec->field_04 = 0;
    rec->cursor_index = data_8016e694[obj->side];
    rec->confirm_mask = 0;
    rec->second_cursor_index = data_8016e696;
    rec->field_0b = 0;
    rec->slot_a_flag = 0;
    rec->slot_b_flag = 0;
    rec->sequence_index = 0;
    rec->slot_c_flag = 0;
    rec->slot_d_flag = 0;
    rec->field_17 = 0;
    rec->field_18 = 0;
}
