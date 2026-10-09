/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3b4c_slot04_01(Object *obj);
extern ObjectFn data_801bf288_slot04_01[];

void func_801b41c4_slot04_01(Object *obj) {
    data_801bf288_slot04_01[obj->field_07](obj);
    func_801b3b4c_slot04_01(obj);
}
