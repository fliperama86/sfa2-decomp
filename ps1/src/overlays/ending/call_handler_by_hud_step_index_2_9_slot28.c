/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_8003dbd8_slot28[])(Object *);

void call_handler_by_hud_step_index_2_9_slot28(Object *obj) {
    data_8003dbd8_slot28[data_8018f5a0->step_index_2](obj);
}
