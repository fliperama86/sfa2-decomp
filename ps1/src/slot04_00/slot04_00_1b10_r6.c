/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801bfe7c_slot04_00[];

void func_801b20a8_slot04_00(Object *obj) {
    obj->field_07++;
    func_80145d20(obj);
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, 0x2a);
}

void func_801b2104_slot04_00(Object *obj) {
    if ((s16)obj->field_3a & 0xff00) {
        obj->field_07++;
        if (obj->field_4b == 0) {
            obj->field_165 = 0xff;
        } else {
            obj->field_165 = 1;
        }
        func_80120554(obj, obj->side, 0x31c);
        func_80148494(obj, 0x1b, 0x3c);
    }
    func_80130efc(obj);
}

void func_801b2188_slot04_00(Object *obj) {
    int i = 0;

    func_80130efc(obj);
    if (((s16)obj->field_3a & 0xff00) == 0) {
        obj->field_07++;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
            i = (obj->field_12a >> 1) + 1;
        }
        obj->field_27b = data_801bfe7c_slot04_00[i];
    }
}
