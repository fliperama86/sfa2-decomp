/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80044618_slot28[];

void func_80021394_slot28(Object *obj) {
}

void func_8002139c_slot28(Object *obj) {
    data_80044618_slot28[obj->field_05](obj);
    func_80131094(obj);
}
