/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142a14(Object *object);
extern ObjectFn data_801c3b60_slot04_08[];

void func_801b0438_slot04_08(Object *obj) {
    func_80142a14(obj);
}

void func_801b0458_slot04_08(Object *obj) {
    data_801c3b60_slot04_08[obj->field_07](obj);
}
