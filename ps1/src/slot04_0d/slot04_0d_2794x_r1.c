/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c2808_slot04_0d[];
extern u8 data_801c2820_slot04_0d[];

void func_801b2794_slot04_0d(Object *obj) {
    int i;
    u8 t;
    u16 a;
    s32 *tbl = data_801c2808_slot04_0d;

    t = obj->field_3a;
    obj->field_249 = 2;
    func_80130efc(obj);
    if ((t & 0x80) != 0) {
        i = obj->field_12a;
        obj->field_54 = -0x8000;
        obj->field_58 = 0x6000;
        obj->field_07++;
        obj->field_4c = tbl[i];
        obj->field_50 = -tbl[i + 1];
    } else {
        a = obj->field_3a;
        i = a & 0xff;
        if (i == 2) {
            obj->field_3a = a & 0xff00;
            obj->field_4c = 0x80000;
            obj->field_54 = -0x8000;
        }
        if ((u8)obj->field_3a == 0) {
            i = 0;
            if (obj->field_165 != 0) {
                obj->field_165 = 0;
                if (obj->field_4b == 0) {
                    obj->other->field_6b = 0xa;
                    i = obj->field_12a >> 1;
                    i += 1;
                }
                ((Slot04bObj *)obj)->field_27b = data_801c2820_slot04_0d[i];
            }
            i = obj->field_4c;
            if (obj->field_0b == 0) {
                i = -i;
            }
            *(s32 *)&obj->field_10 = i + *(s32 *)&obj->field_10;
            obj->field_4c = obj->field_4c + obj->field_54;
            if (obj->field_4c < 0) {
                obj->field_4c = 0;
                obj->field_54 = 0;
            }
        }
    }
}
