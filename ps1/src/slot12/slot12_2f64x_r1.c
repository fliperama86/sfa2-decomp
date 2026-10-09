/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8002b6b0_slot12[];
extern u16 data_800230a0_slot12[];
extern u8 data_80023120_slot12[];
extern u16 data_80022ea0_slot12[];
extern u8 *data_8002b620_slot12;
extern u16 *data_8002b624_slot12;
extern u16 *data_8002b628_slot12;
extern u8 *data_8002b62c_slot12;
void func_80013180_slot12(void);
void func_80013224_slot12(Object *obj, u8 a, s16 x, s16 y);
void func_80013450_slot12(Object *obj, int a);

/* The locals fp and y are 16 bits wide and the loop counters row and col are signed bytes: as int, this function differs from the original in 4, 9, 11 and 58 instruction slots. The mask 0x3f of s is a statement of its own: written in the index, 4 instruction slots differ. The local m holds 1 << col: without it, 6 instruction slots differ. */
void func_80012f64_slot12(Object *obj) {
    Object *q;
    int s;
    s16 b0;
    s16 b2;
    s16 fp;
    int x;
    s16 y;
    int v;
    s8 row;
    s8 col;
    int w;
    int m;

    q = game_state.field_154;
    obj->field_5c = 0;
    data_8002b620_slot12 = data_8002b6b0_slot12;
    data_8002b628_slot12 = data_800230a0_slot12;
    s = (s16)q->field_b2;
    data_8002b62c_slot12 = data_80023120_slot12 + s * 8;
    s = (s16)q->field_b0;
    s += 0x20;
    s &= 0x7f;
    if (s >= 0x40) {
        s = 0;
    }
    b0 = q->field_b0;
    if (b0 >= 0x60) {
        func_80013450_slot12(obj, s);
        return;
    }
    b2 = q->field_b2;
    if (b2 == 0) {
        s = 0;
    }
    s &= 0x3f;
    fp = data_800230a0_slot12[s];
    s = b0;
    if (b2 == 0 && s < 0x20) {
        s = 0x20;
    }
    s &= 0x7c;
    data_8002b624_slot12 = data_80022ea0_slot12 + s * 2;
    y = -(fp << 2);
    v = -0x2a;
    func_80013180_slot12();
    for (row = 0; row < 8; row++) {
        x = v;
        s = *data_8002b62c_slot12++;
        w = *data_8002b624_slot12++;
        for (col = 7; col >= 0; col--) {
            m = 1 << col;
            if (s & m) {
                func_80013224_slot12(obj, w, x, y);
                obj->field_5c++;
            }
            x += 0xc;
        }
        y += fp;
        v -= 2;
    }
    func_80013224_slot12(obj, 0xff, 0, 0);
}
