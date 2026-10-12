/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_8002c728_slot28[])(Object *);

void ignore_object_empty_2_slot28(Object *obj) {
}

void call_handler_by_hud_step_index_2_2_slot28(Object *obj) {
    data_8002c728_slot28[data_8018f5a0->step_index_2](obj);
}
