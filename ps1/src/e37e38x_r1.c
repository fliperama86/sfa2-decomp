/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80137e38(Object *object) {
    object->field_80 = 1;
    object->field_06++;
    object->field_0b ^= 1;
    func_801380f0(object);
    object->field_46 = (u8)object->field_46 | 0x200;
    func_80131094(object);
}
