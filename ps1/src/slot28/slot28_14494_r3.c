/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8004ab88_slot28[])(Object *);

void func_8002468c_slot28(Object *obj) {
    data_8004ab88_slot28[obj->field_05](obj);
    func_80131094(obj);
    obj->field_01 = 1;
}
