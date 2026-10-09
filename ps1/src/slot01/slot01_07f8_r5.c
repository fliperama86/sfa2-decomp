/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_80023d00_slot01[];

void func_8001231c_slot01(Object *obj, u8 a) {
    func_80130768(obj, a, data_80023d00_slot01);
}
