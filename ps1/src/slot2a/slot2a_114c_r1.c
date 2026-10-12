/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot2aBank data_801e2fdc_slot2a[];
extern u32 data_801e515c_slot2a;
extern void (*data_801e2300_slot2a[])(Object *);
extern void (*data_801e230c_slot2a[])(Object *);
void call_state_1_table_then_link_if_health_nonzero_slot2a(Object *obj);
void link_selector_records_into_ordering_table_slot2a(Object *obj);

void call_handler_by_state_0_2_slot2a(Object *obj) {
    data_801e2300_slot2a[obj->state_0](obj);
}

void init_object_fields_and_advance_states_slot2a(Object *obj) {
    obj->ot_depth = 1;
    obj->pos_x = 0x68;
    obj->field_01 = 0;
    obj->health = 0;
    obj->field_20 = 0;
    obj->y_scale = 0;
    obj->table_index_2 = 0;
    obj->field_0c = 0;
    obj->state_0++;
    obj->state_1++;
    if (obj->active_state != obj->table_index) {
        obj->pos_x = 0x10;
    }
    call_state_1_table_then_link_if_health_nonzero_slot2a(obj);
}
