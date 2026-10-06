/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Rec data_8002bca4_slot12[];
extern u8 data_8002bfb4_slot12[][10];
void func_80015f00_slot12(u8 *src, u8 *dst, int mode);

void func_80015dfc_slot12(Object *obj, int a) {
    Slot12Rec *e;
    u8 *fr;

    fr = (u8 *)ptr_8019040c;
    fr += (s16)obj->field_5c << 4;
    e = &data_8002bca4_slot12[obj->field_03];
    e->field_08 = 0x10;
    e->field_09 = 0x10;
    e->field_0a = 2;
    e->field_0b = 0x12;
    e->field_00 = 0;
    if (a == 1) {
        e->field_0b = 0x1d;
    }
    fr += 4;
    func_80015f00_slot12(fr, data_8002bfb4_slot12[obj->field_03], 0);
    e->field_0c = data_8002bfb4_slot12[obj->field_03];
    e->field_04 = obj->field_76 + 0x30;
    e->field_06 = obj->field_78;
    e->field_04 = e->field_04 - 8;
    e->field_06 = e->field_06 - 8;
}
