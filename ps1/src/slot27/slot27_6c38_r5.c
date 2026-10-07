/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot27Recb388 data_8002b388_slot27[2][0x118];
extern Slot27Recf0c8 data_8002f0c8_slot27;
extern ObjectFn data_800292d4_slot27[];
extern u8 data_801a3fe4[];
void func_800178c8_slot27(Tx *tx);

void func_80017064_slot27(Object *unused) {
    int i;
    int j;
    Slot27Recb388 *r;
    Slot27Recf0c8 *g = &data_8002f0c8_slot27;
    u16 *a;
    u16 *b;
    u16 t;

    data_8002f0c8_slot27.field_8a = 0xf;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 0x118; j++) {
            r = &data_8002b388_slot27[i][j];
            func_800178c8_slot27((Tx *)r);
            r->field_10 = 0x80;
            r->field_11 = 0x80;
            r->field_12 = 0x80;
        }
    }
    g->field_50 = 0x801b0000;
    g->field_1e = 0x5800;
    g->field_58 = 0x200;
    g->field_5c = 0x500;
    g->field_10 = 0x2000000;
    g->field_28 = 0xffd00000;
    data_800292d4_slot27[game_state.field_40]((Object *)g);
    func_80157d9c(0);
    a = (u16 *)data_801a3fe4;
    b = a - 0xa00;
    for (i = 0; i < 0x20; i++) {
        for (j = 1; j < 0x10; j++) {
            t = a[i * 16 + j];
            if (t == 0) {
                t = 0x421;
            }
            b[i * 16 + j] = t;
            a[i * 16 + j] = t;
        }
    }
    func_80137220(1, 1);
}
