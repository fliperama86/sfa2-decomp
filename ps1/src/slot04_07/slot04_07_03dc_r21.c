/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b414c_slot04_07(Object *obj);
extern ObjectFn data_801c20f8_slot04_07[];

void func_801b4444_slot04_07(Object *obj) {
    data_801c20f8_slot04_07[obj->field_07](obj);
    func_801b414c_slot04_07(obj);
}
