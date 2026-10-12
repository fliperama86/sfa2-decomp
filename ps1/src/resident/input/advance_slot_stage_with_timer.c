/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void advance_slot_stage_with_timer(Object *object, int index, int arg) {
    Slot *slot;
    object->slots[(u8)index].stage++;
    arg = 0x20;
    if (object->field_7e != 0) arg = 5;
    object->slots[(u8)index].timer = arg;
    slot = &object->slots[(u8)index];
    *(u16 *)&slot->repeat_count = 0;
    clear_global_flag_ignoring_object(object);
}
