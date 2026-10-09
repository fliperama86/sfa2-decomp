/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800469e4_slot28[])(Object *);

void func_800220d4_slot28(Object *obj) {
}

void func_800220dc_slot28(Object *obj) {
    data_800469e4_slot28[obj->field_05](obj);
    func_80131094(obj);
    obj->field_01 = 1;
}
