/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142a14(Object *object);
extern ObjectFn data_801c1390_slot04_05[];

void func_801b09e8_slot04_05(Object *obj) {
    func_80142a14(obj);
}

void func_801b0a08_slot04_05(Object *obj) {
    data_801c1390_slot04_05[obj->field_07](obj);
}
