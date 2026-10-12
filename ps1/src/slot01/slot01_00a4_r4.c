/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80055f40_slot01;
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
        t = game_state.timer_or_bits - 1;
        game_state.timer_or_bits = t;
        if ((s16)t >= 0) {
            return;
        }
    }
    if (game_state.active_flag != 0) {
        func_8014f4d4(6, 2);
        rect.x = 0x70;
        rect.y = 0x1e0;
        rect.w = 0x10;
        rect.h = 0x20;
        func_80158028(&rect, data_801a37e4);
        func_80157d9c(0);
        func_8001180c_slot01();
        data_80055f40_slot01->active_count = 0;
        func_800117b8_slot01();
        func_80010724_slot01();
        func_8001180c_slot01();
        data_8018f5a0->phase_index = 3;
        data_8018f5a0->step_index = 0;
        game_state.countdown_2 = 0;
        data_8018f5a0->substep_index = 0;
        game_state.timer_or_bits = 0;
        data_8018f5a0->step_index_2 = 0;
        game_state.countdown_timer_2 = 0;
        data_8018f5a0->pass_index = 0;
        game_state.field_cc = 0;
        data_8018f5a0->pass_index = 0;
        game_state.status_flags = 0;
        game_state.field_80 = 0;
        game_state.field_09 = 1;
    } else {
        data_8018f5a0->substep_index++;
        game_state.field_09 = 1;
    }
}
