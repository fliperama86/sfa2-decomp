/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot04_07Rec1fdc data_801c1fdc_slot04_07[];
extern Slot04_07Rec1ff4 data_801c1ff4_slot04_07[];

int func_801b41c4_slot04_07(Object *obj);

void func_801b2b2c_slot04_07(Object *obj) {
    int k;

    func_80130efc(obj);
    if (!(obj->field_3a & 0x80)) {
        obj->field_45 = 1;
        if (func_801b41c4_slot04_07(obj) < 0) {
            if ((u8)obj->field_3a != 0) {
                k = 0;
                if (obj->field_49 != 0) {
                    k = 6;
                }
                obj->field_4c = 0;
                obj->field_54 = 0;
                obj->field_07++;
                func_801307e0(obj, data_801c1fdc_slot04_07[(k + obj->field_12a) >> 1].value);
            }
        }
    }
}

void func_801b2bd8_slot04_07(Object *obj) {
    int y;

    func_80130efc(obj);
    *(s32 *)&obj->field_14 -= obj->field_50;
    y = obj->field_70;
    obj->field_50 += obj->field_58;
    if (obj->pos_y < y) {
        func_80130efc(obj);
    } else {
        obj->field_07++;
        obj->pos_y = y;
        obj->field_14 = 0;
        obj->field_45 = 0;
        obj->field_17b = 0;
        func_801209c4(obj);
        y = 0;
        if (obj->field_49 != 0) {
            y = 6;
        }
        y += obj->field_12a;
        func_801307e0(obj, data_801c1ff4_slot04_07[y >> 1].value);
    }
}
