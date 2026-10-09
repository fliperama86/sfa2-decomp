/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80029324_slot27;
extern HudState *data_8018f5a0;
void func_8011ea68(u8 *unused);
void func_800105ec_slot27(void);

void func_80010100_slot27(void) {
    Rect r;
    s16 i;
    func_80151020(0x307);
    game_state.field_6d = 0;
    func_8011eae4();
    r.x = 0x380;
    data_80029324_slot27 = 0;
    data_8018db10 = 0;
    r.y = 0x100;
    r.w = 0x40;
    r.h = 0x100;
    func_80157fc4(&r, ((u8 *)0x800e5000));
    r.x = 0x40;
    r.y = 0x1e0;
    r.w = 0x10;
    r.h = 0x20;
    func_80157fc4(&r, ((u8 *)0x800ed800));
    func_8011ea68((u8 *)0x800ee400);
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[0][i] = ((u16 *)0x800e0c00)[i];
        data_801a27e4_rows[5][i] = ((u16 *)0x800e0c00)[i];
    }
    func_80137220(0, 0);
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[1][i] = ((u16 *)0x800e0000)[i];
        data_801a27e4_rows[6][i] = ((u16 *)0x800e0000)[i];
    }
    func_80137220(1, 1);
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[2][i] = ((u16 *)0x800e0400)[i];
        data_801a27e4_rows[7][i] = ((u16 *)0x800e0400)[i];
    }
    func_80137220(2, 2);
    for (i = 0; i < 0x200; i++) {
        data_801a27e4_rows[3][i] = ((u16 *)0x800e0800)[i];
        data_801a27e4_rows[8][i] = ((u16 *)0x800e0800)[i];
    }
    func_80137220(3, 3);
    for (i = 0; i < 0xa0; i++) {
        data_801a27e4_rows[2][0x160 + i] = ((u16 *)0x800ed400)[i];
        data_801a27e4_rows[7][0x160 + i] = ((u16 *)0x800ed400)[i];
    }
    func_80137220(2, 2);
    data_801aa544[0].field_12 = 0x248;
    ((u16 *)&data_801aa544[0].field_14)[1] = 0x100;
    data_801aa544[1].field_12 = 0x148;
    ((u16 *)&data_801aa544[1].field_14)[1] = 0;
    data_801aa544[2].field_12 = 0x48;
    ((u16 *)&data_801aa544[2].field_14)[1] = 0x700;
    game_state.field_ce = 0x248;
    game_state.field_d0 = 0x100;
    game_state.field_d2 = 0x148;
    game_state.field_d4 = 0;
    game_state.field_d6 = 0x48;
    game_state.field_d8 = 0x700;
    game_state.field_da = 0;
    func_80157d9c(0);
    if (game_state.field_05 != 0) {
        data_8018f5a0->field_4e++;
        game_state.field_05 = 0;
        func_800105ec_slot27();
    } else {
        HudState *h = data_8018f5a0;
        h->field_50++;
        h->field_52 = 0;
        h->field_54 = 0;
        game_state.field_c6 = 0;
        game_state.field_c8 = 0;
        game_state.field_b0 = 0;
        game_state.field_b1 = 0xff;
        game_state.field_b2 = 0;
    }
}
