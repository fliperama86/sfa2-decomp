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

void call_state_1_table_then_link_if_health_nonzero_slot2a(Object *obj) {
    data_801e230c_slot2a[obj->state_1](obj);
    if ((s16)obj->health != 0) {
        link_selector_records_into_ordering_table_slot2a(obj);
    }
}
