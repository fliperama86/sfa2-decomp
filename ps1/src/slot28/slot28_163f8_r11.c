/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80051828_slot28[])(Object *);

void func_800284d4_slot28(Object *object) {
    data_80051828_slot28[object->field_04](object);
}
