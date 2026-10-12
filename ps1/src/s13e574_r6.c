/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


int call_slot_stage_handler_in_fourth_table_2(Object *object, int index, u8 arg) {
    table_8017ac14[object->slots[(u8)index].stage](object, (u8)index, arg);
    return data_80188f44;
}

void init_slot_fields_and_advance_stage(Object *object, int index, int arg) {
    u8 v = object->slots[(u8)index].stage;
    object->slots[(u8)index].entry_index = 1;
    object->slots[(u8)index].repeat_count = 0;
    object->slots[(u8)index].direction = 1;
    object->slots[(u8)index].timer = 0;
    object->slots[(u8)index].level = 1;
    object->slots[(u8)index].field_06 = 0;
    object->slots[(u8)index].stage = v + 1;
    update_slot_three_input_counters(object, (u8)index, (u8)arg);
}
