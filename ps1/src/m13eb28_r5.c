/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void set_slot_random_timer_and_advance(Object *object, int index, int unused) {
    Bytes32 table = data_8016d9ac;
    object->slots[(u8)index].entry_index++;
    object->slots[(u8)index].timer = table.b[get_random_byte() & 0x1f];
    func_8013f2c8(object);
}

void set_slot_random_timer(Object *object, u8 index, u8 unused) {
    Bytes32 table = data_8016d9ac;
    object->slots[index].timer = table.b[get_random_byte() & 0x1f];
    func_8013f2c8(object);
}
