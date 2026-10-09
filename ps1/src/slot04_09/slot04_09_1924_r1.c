/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c7c44_slot04_09[];
void func_801b536c_slot04_09(Object *obj);

void func_801b1924_slot04_09(Object *obj) {
    int n = 0x25;

    obj->field_17b = 1;
    obj->field_225 = 1;
    obj->field_07++;
    if (obj->field_246 == 0) {
        obj->field_225 = 0;
        func_80141f28(obj, 4);
    }
    func_80138ae8(&game_state, obj);
    func_801204f4(obj, obj->side, 4);
    if (obj->field_49 != 0) {
        n = 0x43;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + n);
}

void func_801b19c8_slot04_09(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07++;
        obj->field_3a = obj->field_3a & 0xff00;
        func_801b536c_slot04_09(obj);
        obj->field_46 = data_801c7c44_slot04_09[obj->field_12a >> 1];
    }
    func_80130efc(obj);
}

void func_801b1a44_slot04_09(Object *obj) {
    s16 t;

    if ((s16)obj->field_3a & 0x8000) {
        obj->field_a0 = 0;
        func_801312b8(obj);
    } else {
        t = obj->field_46;
        if (t != 0) {
            t = t - 1;
            obj->field_46 = t;
            if (t != 0) {
                goto tail;
            }
            obj->field_17b = 0;
        }
        func_80142adc(obj);
    tail:
        func_80130efc(obj);
    }
}
