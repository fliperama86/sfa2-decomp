/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 box_margin[];
extern u8 data_801c0670_slot04_0a[];

void func_801b243c_slot04_0a(Object *obj) {
    int m;
    int d;
    int a;
    Object *pl = &player_left;

    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 < 0) {
        obj->field_07++;
        func_801204f4(obj, obj->side, 0xf);
        m = *(s16 *)box_margin;
        a = ((Slot04aObj *)obj)->field_102;
        d = *(s16 *)((u8 *)data_801c0670_slot04_0a + (a & 0xfe));
        if (obj->field_0b == 0) {
            m += 0x180;
            d = -d;
        }
        obj->pos_x = d + m;
        if (pl->field_164 != 0) {
            a = -1;
            if (obj->field_0b != 0) {
                a = 1;
            }
            *(u16 *)&player_left.pos_x += a;
            player_left.field_164 = 0;
        }
        a = 0x2f;
        if (obj->field_49 != 0) {
            a = 0x4c;
        }
        func_801307e0(obj, a);
    }
}
