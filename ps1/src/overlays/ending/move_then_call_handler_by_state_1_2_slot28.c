/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void update_vertical_bounce_motion_slot28(Object *obj);
extern void (*data_80035400_slot28[])(Object *);

void move_then_call_handler_by_state_1_2_slot28(Object *obj) {
    update_vertical_bounce_motion_slot28(obj);
    data_80035400_slot28[obj->state_1](obj);
}
