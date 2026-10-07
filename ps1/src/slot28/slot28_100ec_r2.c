/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_800422e4_slot28[];
extern void (*data_80042308_slot28[])(Object *);

void func_8002036c_slot28(Object *obj) {
    data_800422e4_slot28[obj->field_03](obj);
}

void func_800203ac_slot28(Object *obj) {
    data_80042308_slot28[obj->field_05](obj);
    func_80131094(obj);
    obj->field_01 = 1;
}
