/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Object *data_8002d568_slot12;
extern Object *data_8002d56c_slot12;
extern int data_8002d578_slot12;
extern unsigned data_80028a98_slot12;
extern u16 box_margin[];
Block172 *func_8011f1e0(void);
void func_8011abe4(void);
void func_80135c88(void);
void func_8012818c(void);
void func_800113e4_slot12(void);
void func_80010a94_slot12(void);

void func_80010b8c_slot12(void) {
    Rect r;
    Block172 *b;
    Object *o;
    int one;
    int cnt;
    if (game_state.field_44 & 0x80) {
        while (game_state.field_f0 != 0) {
            func_80138164();
            func_8011abe4();
            func_80135c88();
            func_801192bc(1);
        }
        func_8012818c();
        while (game_state.field_f0 != 0) {
            func_80138164();
            func_8011abe4();
            func_80135c88();
            func_801192bc(1);
        }
    }
    data_80190568 = 1;
    data_8018f5a0->field_4c = data_8018f5a0->field_4c + 1;
    o = game_state.field_154;
    game_state.field_c6 = 0x1f;
    game_state.field_d2 = 0x40;
    game_state.field_ab = 0;
    game_state.field_4f = 0;
    game_state.field_65 = 0;
    game_state.field_ce = 0;
    game_state.field_d0 = 0;
    game_state.field_d4 = 0x100;
    game_state.field_d6 = 0;
    game_state.field_d8 = 0;
    game_state.field_da = 0x100;
    data_8002d56c_slot12 = o;
    data_8002d568_slot12 = &player_left + (o->side ^ 1);
    game_state.field_225 = 0;
    func_8014f4d4(1, 0x607);
    while (data_80190949 == 0 || game_state.field_f0 != 0) {
        func_80138164();
        func_8011abe4();
        func_80135c88();
        func_801192bc(1);
    }
    r.x = 0x180;
    r.y = 0x100;
    r.w = 0x40;
    r.h = 0x100;
    func_80157fc4(&r, (u8 *)0x80043344);
    func_80157d9c(0);
    func_800113e4_slot12();
    one = 1;
    data_80028a98_slot12 = one;
    b = func_8011f1e0();
    if (b != 0) {
        b->field_00 = 1;
        b->field_02 = 0x26;
        b->field_03 = 0;
        if (game_state.field_44 & 0x80) {
            data_8002d578_slot12 = one;
            b = func_8011f1e0();
            if (b != 0) {
                b->field_02 = 0x7b;
                b->field_00 = 1;
                b->field_03 = 1;
                b->field_7a = 0;
                b->field_7c = 0x1e0;
            }
            b = func_8011f1e0();
            if (b != 0) {
                b->field_02 = 0x7b;
                b->field_03 = 2;
                b->field_00 = 1;
                b->field_7a = 0;
                b->field_7c = 0x1e0;
            }
            while (data_80028a98_slot12-- > 0) {
                func_80138164();
                func_8011abe4();
                func_80135c88();
                func_801192bc(1);
            }
            func_80010a94_slot12();
        }
    }
    if (data_8002d578_slot12 != 0) {
        data_8002d56c_slot12->pos_x = box_margin[0] + 0xc0;
    } else {
        data_8002d568_slot12->pos_x = box_margin[0] + 0xc0;
    }
}
