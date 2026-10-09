/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142a14(Object *s);
extern ObjectFn data_801c7bcc_slot04_09[];

void func_801b0a34_slot04_09(Object *obj) {
    func_80142a14(obj);
}

void func_801b0a54_slot04_09(Object *obj) {
    data_801c7bcc_slot04_09[obj->field_07](obj);
}
