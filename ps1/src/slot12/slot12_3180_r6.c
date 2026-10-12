/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void fill_four_table_bytes_slot12(Object *o) {
    Slot12Obj *obj = (Slot12Obj *)o;
    int i;

    obj->tick_counter = 0;
    for (i = 0; i < 4; i += 2) {
        obj->tbl[i] = i + 1;
        obj->tick_counter++;
        obj->tbl[i + 1] = i - 0x7f;
        obj->tick_counter++;
    }
}
