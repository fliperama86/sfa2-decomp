/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801c4550_slot04_04[];
extern u8 data_801c4554_slot04_04[];
extern u8 data_801c4590_slot04_04[];
void call_state_1_table_handler_then_cull_check_2_slot04_04(Object *obj);

void init_child_object_from_parent_slot04_04(Object *obj) {
    Object *parent;

    obj->variant_index_2 = 0xff;
    obj->state_0 = obj->state_0 + 1;
    parent = obj->parent;
    obj->ot_depth = 0;
    obj->field_49 = parent->field_49;
    obj->field_1c = parent->field_1c;
    ((Slot04bObj *)obj)->data_table_ptr = data_801c4590_slot04_04;
    ((Slot04bObj *)obj)->data_pointer = (int)data_801c4554_slot04_04;
    obj->active_state = 0;
    obj->health = 0xff;
    set_sequence_entry(obj, data_801c4550_slot04_04[0]);
    call_state_1_table_handler_then_cull_check_2_slot04_04(obj);
}
