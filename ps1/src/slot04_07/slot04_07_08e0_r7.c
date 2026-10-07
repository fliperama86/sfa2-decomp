/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0e8c_slot04_07(Object *obj) {
    s16 t = obj->field_3a;
    u16 *q;
    FrameRecord *fr;

    if (t & 0x8000) {
        func_80131468(obj);
    } else {
        if ((t & 0xff) != 0) {
            if (obj->field_0b == 0) {
                *(s32 *)&obj->field_10 -= obj->field_4c;
            } else {
                *(s32 *)&obj->field_10 += obj->field_4c;
            }
            obj->field_4c += obj->field_54;
            if (obj->field_4c < 0) {
                q = (u16 *)obj->sequence;
                q += 6;
                obj->sequence = (SequenceStep *)q;
                obj->field_38 = q[0];
                obj->field_3a = q[1];
                fr = obj->frames + obj->sequence->frame_index;
                obj->frame = fr;
                obj->field_4a = fr->field_0d;
                obj->field_80 = 1;
            }
        }
        func_80130efc(obj);
    }
}

void func_801b0f74_slot04_07(Object *obj) {
    if ((obj->field_3a & 0xff) != 0) {
        obj->field_4c = 0xfffd8000;
        obj->field_58 = -0x5000;
        obj->field_50 = 0x58000;
        obj->field_54 = 0;
        obj->field_07++;
        if (obj->field_0b == 0) {
            obj->field_4c = 0x28000;
        }
        obj->field_45 = 1;
    }
    func_80130efc(obj);
}
