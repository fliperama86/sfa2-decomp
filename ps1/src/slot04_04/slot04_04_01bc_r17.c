/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c4210_slot04_04[];
void func_801b17c4_slot04_04(Object *obj);
void func_801b1a54_slot04_04(Object *obj);

void func_801b16d0_slot04_04(Object *obj) {
    int a;

    obj->field_17b = 1;
    obj->field_07++;
    func_80141f28(obj, 4);
    func_80138ae8(&game_state, obj);
    obj->field_254 = 0;
    a = 0x21;
    if (obj->field_49 != 0) {
        a = 0x40;
    }
    a += obj->field_12a;
    func_801307e0(obj, a);
}

void func_801b1744_slot04_04(Object *obj) {
    if (*(u8 *)&obj->field_3a == 0) {
        func_80130efc(obj);
    } else {
        obj->field_46 = 0xf;
        obj->field_07++;
        if (obj->field_cd == 0) {
            func_801b17c4_slot04_04(obj);
        } else {
            obj->field_46 = data_801c4210_slot04_04[obj->field_129];
            func_801b1a54_slot04_04(obj);
        }
    }
}
