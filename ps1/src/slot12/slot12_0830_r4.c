/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8002d568_slot12;
extern Object *data_8002d56c_slot12;
extern int data_8002d578_slot12;
extern unsigned data_80028a98_slot12;
void func_800113e4_slot12(void);
void func_80010a94_slot12(void);

void func_80010b8c_slot12(void) {
    Rect r;
    Block172 *b;
    Object *o;
    int one;
    int cnt;
    if (game_state.field_44 & 0x80) {
        while (game_state.wait_flag != 0) {
            func_80138164();
            func_8011abe4();
            func_80135c88();
            func_801192bc(1);
        }
        func_8012818c();
        while (game_state.wait_flag != 0) {
            func_80138164();
            func_8011abe4();
            func_80135c88();
            func_801192bc(1);
        }
    }
    data_80190568 = 1;
    data_8018f5a0->step_index = data_8018f5a0->step_index + 1;
    o = game_state.object;
    game_state.countdown_2 = 0x1f;
    game_state.field_d2 = 0x40;
    game_state.status_flags = 0;
    game_state.field_4f = 0;
    game_state.field_65 = 0;
    game_state.cam_x = 0;
    game_state.cam_y = 0;
    game_state.field_d4 = 0x100;
    game_state.cam_param_a = 0;
    game_state.cam_param_b = 0;
    game_state.field_da = 0x100;
    data_8002d56c_slot12 = o;
    data_8002d568_slot12 = &player_left + (o->side ^ 1);
    game_state.loop_mode = 0;
    func_8014f4d4(1, 0x607);
    while (data_80190949 == 0 || game_state.wait_flag != 0) {
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
        b->active_flag = 1;
        b->handler_index = 0x26;
        b->block_mode = 0;
        if (game_state.field_44 & 0x80) {
            data_8002d578_slot12 = one;
            b = func_8011f1e0();
            if (b != 0) {
                b->handler_index = 0x7b;
                b->active_flag = 1;
                b->block_mode = 1;
                b->clut_x = 0;
                b->clut_y = 0x1e0;
            }
            b = func_8011f1e0();
            if (b != 0) {
                b->handler_index = 0x7b;
                b->block_mode = 2;
                b->active_flag = 1;
                b->clut_x = 0;
                b->clut_y = 0x1e0;
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
