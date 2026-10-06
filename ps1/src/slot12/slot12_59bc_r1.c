/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Rec data_8002bcc4_slot12[];
extern u8 data_8002bf64_slot12[][16];
void func_800161ec_slot12(u8 *buf, unsigned int n, int pos, int flags);
void func_80015f00_slot12(u8 *src, u8 *dst, int mode);

void func_800159bc_slot12(Object *obj, int a) {
    Slot12Rec *e;
    FrameRecord *fr;
    u8 buf[16];

    fr = (FrameRecord *)ptr_8019040c;
    fr += (s16)obj->field_5c;
    e = &data_8002bcc4_slot12[obj->field_03];
    if (obj->field_03 == 0) {
        e->field_08 = 0x10;
        e->field_09 = 0x10;
        e->field_0a = 2;
        e->field_00 = 0;
        e->field_0b = 0x18;
        if (a == 1) {
            e->field_0b = 0x1d;
        }
        func_800161ec_slot12(buf, fr->field_0c, 7, 0);
        if (fr->field_0c == 1) {
            func_80015f00_slot12(buf, data_8002bf64_slot12[obj->field_03], 2);
        } else {
            func_80015f00_slot12(buf, data_8002bf64_slot12[obj->field_03], 3);
        }
        e->field_0c = data_8002bf64_slot12[obj->field_03];
        e->field_04 = obj->field_76 + 0x28;
    } else {
        e->field_08 = 8;
        e->field_09 = 0x10;
        e->field_0a = 1;
        e->field_00 = 0;
        e->field_0b = 0x18;
        if (a == 1) {
            e->field_0b = 0x1d;
        }
        func_800161ec_slot12(buf, fr->field_0c, 7, 0);
        if (fr->field_0c == 1) {
            func_80015f00_slot12(buf, data_8002bf64_slot12[obj->field_03], 2);
        } else {
            func_80015f00_slot12(buf, data_8002bf64_slot12[obj->field_03], 3);
        }
        e->field_0c = data_8002bf64_slot12[obj->field_03];
        e->field_04 = obj->field_76 + 0x40;
    }
    e->field_06 = obj->field_78;
    e->field_04 = e->field_04 - 8;
    e->field_06 = e->field_06 - 8;
}
