/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_8004ab04_slot28[])(Object *);
extern void (*data_8004ab14_slot28[])(Object *);

void call_table_handler_by_table_index_5_slot28(Object *obj) {
    data_8004ab04_slot28[obj->table_index](obj);
    obj->field_01 = 1;
}

void call_handler_by_state_1_13_slot28(Object *object) {
    data_8004ab14_slot28[object->state_1](object);
}
