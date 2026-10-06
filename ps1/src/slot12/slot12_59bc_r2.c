/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Rec data_8002bcb4_slot12[];
extern u8 data_8002bf14_slot12[][16];
void func_80016294_slot12(u8 *buf, unsigned int n, int pos, int flags);
void func_80015f00_slot12(u8 *src, u8 *dst, int mode);

void func_80015b90_slot12(Object *obj, int a) {
    Slot12Rec *e;
    u8 *fr;
    u8 buf[16];

    fr = (u8 *)ptr_8019040c;
    fr += (s16)obj->field_5c << 4;
    e = &data_8002bcb4_slot12[obj->field_03];
    if (game_state.field_2bd == 0) {
        e->field_08 = 0x10;
        e->field_09 = 0x10;
        e->field_0a = 2;
        e->field_00 = 0;
        e->field_0b = 0x18;
        if (a == 1) {
            e->field_0b = 0x1d;
        }
        func_80016294_slot12(buf, *(int *)fr, 7, 0);
        func_80015f00_slot12(buf, data_8002bf14_slot12[obj->field_03], 0);
        e->field_0c = data_8002bf14_slot12[obj->field_03];
        e->field_04 = obj->field_76 + 0x70;
        e->field_06 = obj->field_78;
        e->field_04 = e->field_04 - 8;
        e->field_06 = e->field_06 - 8;
    } else if (obj->field_03 == 0) {
        e->field_08 = 0x10;
        e->field_09 = 0x10;
        e->field_0a = 2;
        e->field_00 = 0;
        e->field_0b = 0x18;
        if (a == 1) {
            e->field_0b = 0x1d;
        }
        func_80016294_slot12(buf, *(int *)fr, 7, 0);
        func_80015f00_slot12(buf, data_8002bf14_slot12[obj->field_03], 1);
        e->field_0c = data_8002bf14_slot12[obj->field_03];
        e->field_04 = obj->field_76 + 0x38;
        e->field_06 = obj->field_78 + 0x20;
        e->field_04 = e->field_04 - 8;
        e->field_06 = e->field_06 - 8;
    } else {
        e->field_08 = 8;
        e->field_09 = 0x10;
        e->field_0a = 1;
        e->field_00 = 0;
        e->field_0b = 0x18;
        if (a == 1) {
            e->field_0b = 0x1d;
        }
        func_80016294_slot12(buf, *(int *)fr, 7, 0);
        func_80015f00_slot12(buf, data_8002bf14_slot12[obj->field_03], 0);
        e->field_0c = data_8002bf14_slot12[obj->field_03];
        e->field_04 = obj->field_76 + 0xa0;
        e->field_06 = obj->field_78;
        e->field_04 = e->field_04 - 4;
        e->field_06 = e->field_06 - 8;
    }
}
