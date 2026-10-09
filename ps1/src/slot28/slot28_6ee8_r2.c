/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_800306a4_slot28[];

void func_80016fd8_slot28(Object *obj) {
    data_800306a4_slot28[obj->field_03](obj);
}
