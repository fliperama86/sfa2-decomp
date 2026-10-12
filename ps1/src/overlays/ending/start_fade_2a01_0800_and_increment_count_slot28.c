/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void start_fade_2a01_0800_and_increment_count_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    start_fade_command_2a01_0800_if_idle();
    obj->count += 1;
}
