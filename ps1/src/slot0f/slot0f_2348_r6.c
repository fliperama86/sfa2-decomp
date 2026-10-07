/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern void (*data_800e9724_slot0f[])(void);
extern int data_800f8578_slot0f;
extern Menu data_800f8574_slot0f;
void func_800e32dc_slot0f(void);
void func_800e37a4_slot0f(void);
void func_800e38b4_slot0f(void);
void func_800e3b18_slot0f(void);

void func_800e2880_slot0f(void) {
    HudState *h;
    if (data_800f8578_slot0f != 0) {
        func_800e3b18_slot0f();
    } else {
        data_800e9724_slot0f[data_8018f5a0->field_4e]();
        func_800e38b4_slot0f();
        func_800e37a4_slot0f();
        if ((data_801a696a | data_801a6976) & 0x800) {
            func_800e32dc_slot0f();
            h = data_8018f5a0;
            data_800f8574_slot0f.field_00 = 3;
            h->field_4a = 1;
            h->field_4c = 0;
            game_state.field_c6 = 0;
            h->field_4e = 0;
            game_state.field_c8 = 0;
            h->field_50 = 0;
            game_state.field_ca = 0;
        }
    }
}
