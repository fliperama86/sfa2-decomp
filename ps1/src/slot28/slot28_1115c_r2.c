/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80044608_slot28[])(Object *);
extern ObjectFn data_80044610_slot28[];

void func_800212b0_slot28(Object *obj) {
    data_80044608_slot28[obj->field_03](obj);
    obj->field_01 = 1;
}

void func_80021304_slot28(Object *obj) {
    data_80044610_slot28[obj->field_05](obj);
    func_80131094(obj);
}
