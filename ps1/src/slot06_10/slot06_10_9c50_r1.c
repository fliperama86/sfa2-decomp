/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801ebbb8_slot06_10[];
extern s16 data_801f67f0_slot06_10;
extern s16 data_801f67f4_slot06_10;

void func_801e9da8_slot06_10(Object *obj, Object *other);

void func_801e9c50_slot06_10(Object *obj, Object *other) {
    s16 d;

    if ((s16)obj->field_3a & 0x8000) {
        d = other->pos_x - obj->pos_x;
        if (d < 0) {
            d = -d;
        }
        if ((s16)(d - 0x40) >= 0) {
            func_801e9da8_slot06_10(obj, other);
            if (data_801f67f0_slot06_10 > data_801f67f4_slot06_10) {
                d = 0x10;
                obj->field_05 = 0;
            } else {
                d = 0xe;
                obj->field_05 = 2;
            }
            func_80130700(obj, data_801ebbb8_slot06_10[d]);
            return;
        }
    }
    func_80131094(obj);
}

void func_801e9d24_slot06_10(Object *obj, Object *other) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801e9da8_slot06_10(obj, other);
        if (data_801f67f0_slot06_10 > data_801f67f4_slot06_10) {
            func_80130700(obj, data_801ebbb8_slot06_10[0xf]);
            obj->field_05 = 1;
            return;
        }
    }
    func_80131094(obj);
}
