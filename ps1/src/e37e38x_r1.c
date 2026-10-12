/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80137e38(Object *object) {
    object->sequence_changed_flag = 1;
    object->state_2++;
    object->field_0b ^= 1;
    func_801380f0(object);
    object->field_46 = (u8)object->field_46 | 0x200;
    advance_sequence_checked(object);
}
