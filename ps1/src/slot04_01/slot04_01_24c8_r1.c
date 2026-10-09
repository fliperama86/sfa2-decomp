/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801bf09c_slot04_01[];
extern s32 data_801bf0a0_slot04_01[];
extern u8 data_801bf0b4_slot04_01[];

void func_801b24c8_slot04_01(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    s32 unused[2];
    int i = 0;
    u16 t;

    func_80130efc(obj);
    t = obj->field_3a;
    if (t & 0x80) {
        obj->field_54 = -0x8000;
        obj->field_58 = -0x6000;
        obj->field_249 = 2;
        obj->field_07++;
        obj->field_4c = data_801bf09c_slot04_01[obj->field_12a];
        obj->field_50 = data_801bf0a0_slot04_01[obj->field_12a];
    } else {
        if ((t & 0xff) == 2) {
            obj->field_3a = t & 0xff00;
            obj->field_4c = 0x80000;
            obj->field_54 = -0x8000;
        }
        if (*(u8 *)&obj->field_3a == 0) {
            if (obj->field_165 != 0) {
                obj->field_165 = 0;
                if (obj->field_4b == 0) {
                    obj->other->field_6b = 0xa;
                    i = (obj->field_12a >> 1) + 1;
                }
                obj->field_27b = data_801bf0b4_slot04_01[i];
                func_801204f4(obj, obj->side, 4);
            }
            if (obj->field_0b == 0) {
                *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
            } else {
                *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
            }
            obj->field_4c = obj->field_4c + obj->field_54;
            if (obj->field_4c < 0) {
                obj->field_4c = 0;
                obj->field_54 = 0;
            }
        }
        obj->field_249 = 2;
    }
}
