/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 *data_801e52c8_slot2a;
extern int data_801e52c4_slot2a;
extern void (*data_801e2250_slot2a[])(Object *);

void func_801e0518_slot2a(void);

void func_801e03d0_slot2a(void) {
    u16 *p = data_801e52c8_slot2a;
    if (!(p[0] & 0x8000)) {
        data_8018f5a0->pass_index++;
        game_state.status_flags = 0;
        game_state.field_cc = p[1] & 0x7fff;
    } else {
        func_801e0518_slot2a();
    }
}

void count_down_field_cc_advance_pass_slot2a(void) {
    u16 *c = &game_state.field_cc;
    int t = *c - 1; *c = t;
    if ((s16)*c < 0xdc) {
        data_8018f5a0->pass_index++;
    }
}

void func_801e049c_slot2a(void) {
    if (func_80125268() == 0) {
        u16 *c = &game_state.field_cc;
        int t = *c - 1; *c = t;
        if ((s16)*c >= 0) {
            return;
        }
    }
    data_8018f5a0->pass_index = 3;
    game_state.status_flags = 0xff;
    data_801e52c8_slot2a += 3;
}

void func_801e0518_slot2a(void) {
    set_menu_command_by_code(6, 2);
    data_8018f5a0->pass_index = 0;
    data_801e52c4_slot2a = 0;
    game_state.state_2c_flag = 0;
    game_state.field_84++;
    func_801260ac(0, 0x20, 0);
    func_801260ac(1, 0x20, 0);
    func_801260ac(2, 0x20, 0);
    func_801260ac(3, 0x20, 0);
    queue_five_jobs();
}

void call_handler_by_state_0_slot2a(Object *obj) {
    data_801e2250_slot2a[obj->state_0](obj);
}
