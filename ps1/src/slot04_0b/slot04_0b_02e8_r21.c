/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3e88_slot04_0b(Object *object);

void func_801b3b3c_slot04_0b(Object *object) {
    object->state_2 = object->state_2 + 1;
    func_801b3e88_slot04_0b(object);
    object->countdown = 2;
    advance_sequence_checked(object);
}
