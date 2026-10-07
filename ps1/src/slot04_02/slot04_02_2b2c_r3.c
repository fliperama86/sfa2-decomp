/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c6380_slot04_02[];
extern s32 data_801c6384_slot04_02[];
extern u8 data_801c6398_slot04_02[];

void func_801b2e38_slot04_02(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    s32 unused[2];
    u8 i;
    u16 t;

    obj->field_249 = 2;
    func_80130efc(obj);
    t = obj->field_3a;
    if (t & 0x80) {
        obj->field_54 = -0x8000;
        obj->field_58 = -0x6000;
        obj->field_07++;
        obj->field_4c = data_801c6380_slot04_02[obj->field_12a];
        obj->field_50 = data_801c6384_slot04_02[obj->field_12a];
    } else {
        if ((t & 0xff) == 2) {
            obj->field_3a = t & 0xff00;
            obj->field_4c = 0x80000;
            obj->field_54 = -0x8000;
        }
        if (*(u8 *)&obj->field_3a == 0) {
            if (obj->field_165 != 0) {
                i = 0;
                obj->field_165 = 0;
                if (obj->field_4b == 0) {
                    obj->other->field_6b = 0xa;
                    i = (obj->field_12a >> 1) + 1;
                }
                obj->field_27b = data_801c6398_slot04_02[i];
            } else {
                if (obj->field_0b != 0) {
                    *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
                } else {
                    *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
                }
                obj->field_4c = obj->field_4c + obj->field_54;
                if (obj->field_4c < 0) {
                    obj->field_54 = 0;
                    obj->field_4c = 0;
                }
            }
        }
    }
}
