/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern ObjectFn data_80079a08_slot2b[];
extern Object *data_8007ef28_slot2b;
extern s16 data_8007ef2c_slot2b;

void call_handler_by_state_0_store_parent_slot2b(Object *o) {
    Object *p = o->parent;
    data_8007ef28_slot2b = p;
    data_8007ef2c_slot2b = ((Slot2bObj *)p)->field_70;
    data_80079a08_slot2b[o->state_0](o);
}
