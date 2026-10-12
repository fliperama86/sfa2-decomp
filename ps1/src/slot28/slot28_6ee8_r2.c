/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_800306a4_slot28[];

void call_handler_by_table_index_a_2_slot28(Object *obj) {
    data_800306a4_slot28[obj->table_index](obj);
}
