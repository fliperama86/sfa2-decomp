/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801dd4e4_slot05_06[];

void func_801cc478_slot05_06(Object *obj) {
    obj->field_07++;
    obj->field_17b = 1;
    ref_other.p = obj->other;
    func_8013fab4(obj);
    func_80141f28(obj, 6);
    func_80138ae8(&game_state, obj);
    func_80120554(ref_other.p, ref_other.p->side, 0x31a);
    func_801307e0(obj, 0x19);
}

void func_801cc50c_slot05_06(Object *obj) {
    Object *p;

    if (((Slot04aObj *)obj)->field_3a != 0) {
        obj->field_07++;
        p = obj->other;
        p->field_15b = 1;
        func_80140770(obj, 0x26, 5, *(s16 *)(data_801dd4e4_slot05_06 + (obj->field_12a & 0xfe)), 4, 0, 1);
        if ((s16)p->field_5c < 0) {
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x16;
                obj->field_255 = 6;
                game_state.field_6b = 0;
                func_80147000(obj);
            }
        }
    }
    func_80130efc(obj);
}

void func_801cc5dc_slot05_06(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        obj->field_17b = 0;
        func_801312b8(obj);
    }
}
