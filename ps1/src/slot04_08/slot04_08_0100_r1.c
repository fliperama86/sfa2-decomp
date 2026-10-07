/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c3b38_slot04_08[];
void func_801b4354_slot04_08(Object *o);

void func_801b0100_slot04_08(Object *obj) {
    data_801c3b38_slot04_08[obj->field_06](obj);
    func_801b4354_slot04_08(obj);
}
