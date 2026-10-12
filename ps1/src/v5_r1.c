/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void tick_slot_count_and_timer(Object *object, int index) {
    Slot *slot = &object->slots[(u8)index];
    (*(u16 *)&slot->repeat_count)++;
    object->slots[(u8)index].timer--;
    if (object->slots[(u8)index].timer == 0) object->slots[(u8)index].stage++;
    clear_global_flag_ignoring_object(object);
}
