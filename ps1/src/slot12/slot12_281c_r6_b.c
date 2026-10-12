/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80023170_slot12[])(Object *);
void call_handler_by_state_0_with_tile_update_slot12(Object *obj);

void call_handler_by_state_0_3_slot12(Object *obj) {
    data_80023170_slot12[obj->state_0](obj);
}

void init_object_fields_and_advance_state_0_slot12(Object *obj) {
    obj->field_0c = 1;
    obj->variant_index = 9;
    obj->pos_x = 0xf8;
    obj->field_01 = 0;
    obj->ot_depth = 0;
    obj->field_20 = 0;
    obj->y_scale = 0;
    obj->table_index_2 = 0;
    obj->pos_y = 0x80;
    obj->state_0++;
}
