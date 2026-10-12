/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80137e38(Object *object) {
    object->sequence_changed_flag = 1;
    object->state_2++;
    object->facing_side ^= 1;
    func_801380f0(object);
    object->countdown = (u8)object->countdown | 0x200;
    advance_sequence_checked(object);
}
