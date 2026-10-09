/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c1f80_slot04_07[];

void func_801b2620_slot04_07(Object *obj) {
    obj->field_07++;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_80145d20(obj);
    obj->field_46 = 0x400;
    func_80120554(obj, obj->side, 0x31c);
    func_801307e0(obj, (obj->field_12a >> 1) + 0x49);
}

void func_801b269c_slot04_07(Object *obj) {
    int t = obj->field_46 - 0x100;

    obj->field_46 = t;
    if ((t & 0xff00) == 0) {
        obj->field_07++;
        func_801204f4(obj, obj->side, 5);
        obj->field_46 |= 0x3800;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        func_801483a4(obj, -0x28, 0x3c);
    }
}

void func_801b2724_slot04_07(Object *obj) {
    int t = obj->field_46 - 0x100;
    int i = 0;

    obj->field_46 = t;
    if ((t & 0xff00) == 0) {
        obj->field_165 = 0;
        obj->field_4c = 0;
        obj->field_54 = 0;
        obj->field_07++;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 10;
            i = (obj->field_12a >> 1) + 1;
        }
        obj->field_27b = data_801c1f80_slot04_07[i];
    }
}
