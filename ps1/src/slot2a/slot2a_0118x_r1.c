/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Block172 data_801e2318_slot2a[4];
extern Block172 *data_801e25c8_slot2a[4];
extern u32 data_801e1bdc_slot2a[];
extern u16 *data_801e52c8_slot2a;
extern HudState *data_8018f5a0;

void func_801e0118_slot2a(void) {
    Rect rect;
    u16 pal[16] = {0x0000, 0x7bde, 0x6b5a, 0x6318, 0x5ad6, 0x4a52, 0x4210, 0x39ce,
                   0x294a, 0x2108, 0x18c6, 0x111a, 0x111a, 0x111a, 0x111a, 0x0842};
    Block172 *p;
    Block172 *base;
    int a1;

    data_8018f5a0->field_52++;
    game_state.field_2c = 1;
    game_state.field_ab = 0;
    game_state.field_4f = 0;
    game_state.field_ab = 0xff;
    game_state.field_84 = 0xff;
    game_state.field_89 = 0;
    rect.x = 0x2c0;
    rect.y = 0x80;
    rect.w = 0x10;
    rect.h = 1;
    func_80157fc4(&rect, (u8 *)pal);
    func_80157d9c(0);
    a1 = 0x407;
    if (game_state.field_8b == 0xb) {
        a1 = 0x503;
    }
    game_state.field_225 = 0;
    func_8014f4d4(1, a1);
    while (data_80190949 == 0 || game_state.field_f0 != 0) {
        func_8011a784();
        func_801192bc(1);
    }
    a1 = 0;
    if (game_state.field_80 < 6) {
        int t = game_state.mode & 1;
        a1 = t == 0;
    }
    base = data_801e2318_slot2a;
    p = base;
    if (p) {
        p->field_00 = 1;
        data_801e2318_slot2a[0].field_02 = 0x7f;
        data_801e2318_slot2a[0].field_03 = 0;
        data_801e2318_slot2a[0].field_45 = a1;
        data_801e25c8_slot2a[0] = p;
    }
    p = base + 1;
    if (p) {
        p->field_00 = 1;
        data_801e2318_slot2a[1].field_02 = 0x7f;
        data_801e2318_slot2a[1].field_03 = 1;
        data_801e2318_slot2a[1].field_45 = a1;
        data_801e25c8_slot2a[1] = p;
    }
    p = base + 2;
    if (p) {
        p->field_00 = 1;
        data_801e2318_slot2a[2].field_02 = 3;
        data_801e2318_slot2a[2].field_03 = 0;
        data_801e2318_slot2a[2].field_45 = a1;
        data_801e25c8_slot2a[2] = p;
    }
    p = base + 3;
    if (p) {
        p->field_00 = 1;
        data_801e2318_slot2a[3].field_02 = 3;
        data_801e2318_slot2a[3].field_03 = 1;
        data_801e2318_slot2a[3].field_45 = a1;
        data_801e25c8_slot2a[3] = p;
    }
    data_801e52c8_slot2a = (u16 *)data_801e1bdc_slot2a[game_state.field_8b];
}
