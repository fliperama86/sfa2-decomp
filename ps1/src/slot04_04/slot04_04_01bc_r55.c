/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b4be8_slot04_04(Object *obj);
void func_801b4adc_slot04_04(Object *obj);

void func_801b49d0_slot04_04(Object *obj) {
    int t = 0x1c;

    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 5;
    obj->field_07 = 0;
    func_80141f28(obj, 1);
    obj->field_12f = 0;
    obj->field_67 = 0;
    if (obj->field_48 != 0) {
        t = 0x1d;
    }
    func_801307e0(obj, t);
}

void func_801b4a40_slot04_04(Object *obj) {
    if (obj->field_12f == 0) {
        if (obj->field_67 != 0) {
            func_801b4adc_slot04_04(obj);
        } else if (func_801b4be8_slot04_04(obj) < 0 && !(obj->pos_y < obj->field_70)) {
            func_801209c4(obj);
            func_80131638(obj);
        } else {
            func_80130efc(obj);
        }
    }
}
