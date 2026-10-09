/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801dd4ec_slot05_06[];

void func_801cc61c_slot05_06(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        obj->other->field_249 = 5;
        func_801312b8(obj);
    }
}

void func_801cc660_slot05_06(Object *obj) {
    data_801dd4ec_slot05_06[obj->field_07](obj);
}

void func_801cc6a0_slot05_06(Object *obj) {
    obj->field_07++;
    func_80141f28(obj, 3);
    func_80120554(obj->other, obj->other->side, 0x31a);
    obj->field_0b = 0;
    if (obj->field_cd != 0) {
        if ((s16)(box_margin[0] + 0xc0) < obj->pos_x) {
            obj->field_0b = 1;
        }
    } else if ((obj->field_c2 & 0x8000) == 0) {
        obj->field_0b = 1;
    }
    func_801307e0(obj, 0x18);
}
