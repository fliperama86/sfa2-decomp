/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8004ea50_slot28[])(Object *);
extern ObjectFn data_8004ea60_slot28[];

void func_800260c4_slot28(Object *obj) {
    data_8004ea50_slot28[obj->field_03](obj);
    obj->field_01 = 1;
}

void func_80026118_slot28(Object *obj) {
    data_8004ea60_slot28[obj->field_05](obj);
    func_80131094(obj);
}
