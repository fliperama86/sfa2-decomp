/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c1e38_slot04_07[];
void func_801b414c_slot04_07(Object *o);

void func_801b013c_slot04_07(Object *obj) {
    data_801c1e38_slot04_07[obj->field_06](obj);
    func_801b414c_slot04_07(obj);
}
