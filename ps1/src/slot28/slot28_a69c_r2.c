/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8003743c_slot28[];

void func_8001ab68_slot28(Object *o) {
    o->field_0e = 0;
    o->field_0c = 0;
    o->field_0b = 0;
    o->field_48 = 0;
    o->field_04++;
    func_80130768(o, data_8003743c_slot28[o->field_03], (SequenceStep **)o->box_tables);
}
