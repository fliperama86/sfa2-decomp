/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern u16 *data_801e52c8_slot2a;
extern int data_801e52c4_slot2a;
extern void (*data_801e2250_slot2a[])(Object *);

void func_801e0518_slot2a(void);

void func_801e03d0_slot2a(void) {
    u16 *p = data_801e52c8_slot2a;
    if (!(p[0] & 0x8000)) {
        data_8018f5a0->field_52++;
        game_state.field_ab = 0;
        game_state.field_cc = p[1] & 0x7fff;
    } else {
        func_801e0518_slot2a();
    }
}

void func_801e044c_slot2a(void) {
    u16 *c = &game_state.field_cc;
    int t = *c - 1; *c = t;
    if ((s16)*c < 0xdc) {
        data_8018f5a0->field_52++;
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
    data_8018f5a0->field_52 = 3;
    game_state.field_ab = 0xff;
    data_801e52c8_slot2a += 3;
}

void func_801e0518_slot2a(void) {
    func_8014f4d4(6, 2);
    data_8018f5a0->field_52 = 0;
    data_801e52c4_slot2a = 0;
    game_state.field_2c = 0;
    game_state.field_84++;
    func_801260ac(0, 0x20, 0);
    func_801260ac(1, 0x20, 0);
    func_801260ac(2, 0x20, 0);
    func_801260ac(3, 0x20, 0);
    func_80137b10();
}

void func_801e05b4_slot2a(Object *obj) {
    data_801e2250_slot2a[obj->field_04](obj);
}
