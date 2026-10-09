/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2ef8_slot04_00(Object *obj);
extern ObjectFn data_801bfed0_slot04_00[];

void func_801b2d44_slot04_00(Object *obj) {
    data_801bfed0_slot04_00[obj->field_07](obj);
    func_801b2ef8_slot04_00(obj);
}
