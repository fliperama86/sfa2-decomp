/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4560_slot04_04(Object *obj);
void func_801b42ec_slot04_04(Object *obj);
void func_801b4a40_slot04_04(Object *obj);

void func_801b4258_slot04_04(Object *obj) {
    Object *p = obj->field_3c;

    if (obj->field_03 == 0) {
        *(s32 *)&p->field_2c = 0;
    } else {
        p->field_28 = 0;
    }
    func_8011f38c(obj);
}

void func_801b4294_slot04_04(Object *obj) {
    if (obj->field_128 == 4) {
        func_801b4a40_slot04_04(obj);
    } else if (obj->field_128 != 0) {
        func_801b4560_slot04_04(obj);
    } else {
        func_801b42ec_slot04_04(obj);
    }
}
