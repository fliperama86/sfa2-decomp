/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3aa0_slot04_03(Object *obj);
extern ObjectFn data_801c0528_slot04_03[];

void func_801b0338_slot04_03(Object *obj) {
    data_801c0528_slot04_03[obj->field_07](obj);
    func_801b3aa0_slot04_03(obj);
}
