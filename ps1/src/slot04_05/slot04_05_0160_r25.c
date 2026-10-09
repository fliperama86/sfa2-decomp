/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3d6c_slot04_05(Object *obj);
void func_801b4120_slot04_05(Object *obj);
void func_801b3dac_slot04_05(Object *obj);
void func_801b3fe8_slot04_05(Object *obj);
extern ObjectFn data_801c1658_slot04_05[];

void func_801b3d2c_slot04_05(Object *obj) {
    if (obj->field_128 == 0) {
        func_801b3d6c_slot04_05(obj);
    } else {
        func_801b4120_slot04_05(obj);
    }
}

void func_801b3d6c_slot04_05(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 == 0) {
        func_801b3dac_slot04_05(obj);
    } else {
        func_801b3fe8_slot04_05(obj);
    }
}

void func_801b3dac_slot04_05(Object *obj) {
    data_801c1658_slot04_05[obj->field_07](obj);
}
