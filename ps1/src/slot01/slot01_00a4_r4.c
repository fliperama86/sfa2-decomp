/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Object *data_80055f40_slot01;
extern u8 data_801a37e4[];
void func_800108d4_slot01(void);
void func_8001180c_slot01(void);
void func_800117b8_slot01(void);
void func_80010724_slot01(void);

void func_800105bc_slot01(void) {
    Rect rect;
    int t;

    func_800108d4_slot01();
    if (game_state.field_04 != 0) {
        return;
    }
    if (func_801252f0() == 0) {
        t = game_state.field_c8 - 1;
        game_state.field_c8 = t;
        if ((s16)t >= 0) {
            return;
        }
    }
    if (game_state.field_27 != 0) {
        func_8014f4d4(6, 2);
        rect.x = 0x70;
        rect.y = 0x1e0;
        rect.w = 0x10;
        rect.h = 0x20;
        func_80158028(&rect, data_801a37e4);
        func_80157d9c(0);
        func_8001180c_slot01();
        data_80055f40_slot01->field_00 = 0;
        func_800117b8_slot01();
        func_80010724_slot01();
        func_8001180c_slot01();
        data_8018f5a0->field_4a = 3;
        data_8018f5a0->field_4c = 0;
        game_state.field_c6 = 0;
        data_8018f5a0->field_4e = 0;
        game_state.field_c8 = 0;
        data_8018f5a0->field_50 = 0;
        game_state.field_ca = 0;
        data_8018f5a0->field_52 = 0;
        game_state.field_cc = 0;
        data_8018f5a0->field_52 = 0;
        game_state.field_ab = 0;
        game_state.field_80 = 0;
        game_state.field_09 = 1;
    } else {
        data_8018f5a0->field_4e++;
        game_state.field_09 = 1;
    }
}
