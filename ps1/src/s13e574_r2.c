/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8013e7f0(Object *object, int index, int arg) {
    object->slots[(u8)index].timer--;
    if (object->slots[(u8)index].timer == 0) reset_slot_stage_clear_result(object, (u8)index, (u8)arg);
    else check_slot_high_input_bits_and_advance(object, (u8)index, (u8)arg);
}
