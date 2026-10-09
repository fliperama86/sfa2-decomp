/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b037c_slot04_00(Object *obj);
void func_801b0864_slot04_00(Object *obj);
void func_801b03bc_slot04_00(Object *obj);
void func_801b05c0_slot04_00(Object *obj);

void func_801b033c_slot04_00(Object *obj) {
    if (obj->field_128 == 0) {
        func_801b037c_slot04_00(obj);
    } else {
        func_801b0864_slot04_00(obj);
    }
}

void func_801b037c_slot04_00(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 == 0) {
        func_801b03bc_slot04_00(obj);
    } else {
        func_801b05c0_slot04_00(obj);
    }
}
