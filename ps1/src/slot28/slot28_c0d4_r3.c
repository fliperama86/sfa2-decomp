/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8003a158_slot28[];

void func_8001c640_slot28(Object *o) {
    o->field_0e = 0;
    o->variant_index = 0;
    o->state_0++;
    func_80130768(o, data_8003a158_slot28[o->table_index], ((Slot28Obj *)o)->sequence_ptr);
}
