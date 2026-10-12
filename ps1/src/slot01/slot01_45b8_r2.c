/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80015dcc_slot01[];
extern Object *data_80033b98_slot01;
void set_position_from_parent_table_entry_slot01(Object *obj);

void call_handler_by_state_1_slot01(Object *object) {
    data_80015dcc_slot01[object->state_1](object);
}

void init_object_from_parent_opposite_side_slot01(Object *obj) {
    Object *p;
    data_80033b98_slot01 = obj->parent;
    obj->field_01 = 0;
    obj->field_0c = 1;
    obj->state_1++;
    obj->image_row = data_80033b98_slot01->image_row;
    obj->field_0e = 0;
    obj->ot_depth = 0;
    obj->field_0f = 0;
    obj->countdown = 0x40;
    obj->scroll_factor = 0;
    obj->health = 0xff;
    p = data_80033b98_slot01;
    obj->spawn_x = p->spawn_x;
    obj->spawn_y = p->spawn_y;
    if (p->side != 0) {
        obj->facing_side = 0;
    } else {
        obj->facing_side = 1;
    }
    obj->table_index = obj->variant_index = data_80033b98_slot01->kind;
    set_position_from_parent_table_entry_slot01(obj);
}
