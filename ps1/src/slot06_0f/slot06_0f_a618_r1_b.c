/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801ea96c_slot06_0f(Object *obj);

void func_801ea96c_slot06_0f(Object *obj) {
    obj->countdown = get_random_byte() & 0x3c;
}
