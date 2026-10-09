/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801c3e98_slot04_08;

void func_801b2f6c_slot04_08(Object *obj) {
    int k;

    if (((Slot04aObj *)obj)->field_1c4 != 2) {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
        obj->field_4c = obj->field_4c + obj->field_54;
    }
    if ((s16)obj->field_3a & 0x8000) {
        ((Slot04aObj *)obj)->field_1c4++;
        if (((Slot04aObj *)obj)->field_1c4 == 3) {
            if (obj->field_12a != 2 && obj->other->field_163 == 0) {
                func_801312b8(obj);
            } else {
                obj->field_07 = 4;
                k = 0x4f;
                if (obj->field_12a != 2) {
                    k = 0x50;
                }
                func_801307e0(obj, k);
            }
        } else {
            obj->field_07 = 1;
            func_801307e0(obj, (obj->field_12a >> 1) + 0x43);
        }
    } else {
        func_80130efc(obj);
    }
}

void func_801b3078_slot04_08(Object *obj) {
    int x;

    if ((s16)obj->field_3a & 0x8000) {
        obj->field_4c = 0x80000;
        obj->field_54 = -0xc000;
        obj->field_07++;
        x = 0x160;
        if (obj->field_0b == 0) {
            x = -0x160;
            obj->field_4c = -obj->field_4c;
            obj->field_54 = -obj->field_54;
        }
        data_801c3e98_slot04_08 = x + obj->pos_x;
    }
    func_80130efc(obj);
}
