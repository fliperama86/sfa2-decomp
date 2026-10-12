/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b23a8_slot04_sel(Object *obj, Slot04SelRec9d38 *rec) {
    rec->count = 0;
    rec->kind_index = obj->kind;
    rec->field_04 = 0;
    rec->saved_index_a = data_8016e698[obj->side];
    rec->field_07 = 0;
    rec->saved_index_b = data_8016e698[obj->side + 2];
    rec->field_09 = 0;
    rec->saved_index_c = data_8016e698[4];
    rec->field_0b = 0;
    rec->table_index = data_8016e698[5];
    rec->active_flag = 0;
    rec->field_0e = 0;
    rec->field_0f = 0;
    rec->step_count = 0;
    rec->field_11 = 0;
    rec->field_12 = 0;
    rec->count_2 = 0;
    rec->field_14 = 0;
}
