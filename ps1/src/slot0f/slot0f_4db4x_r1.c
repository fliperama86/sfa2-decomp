/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern u16 data_801903c0;
void func_801280f0(void);
Block172 *func_8011f1e0(void);
void func_800e5a28_slot0f(Object *obj);
void func_800e5470_slot0f(Object *obj);
void func_800e51c8_slot0f(int a);
void func_800e5258_slot0f(void);
void func_800e5608_slot0f(Object *obj);
void func_800e50a8_slot0f(Object *obj);

void func_800e4db4_slot0f(Object *arg) {
    Object *obj;
    Rect r;
    Block172 *b;
    int one;
    data_8018f5a0->field_4a++;
    game_state.field_2bc = 0;
    obj = arg;
    func_801280f0();
    if (obj->field_01 == 0) {
        data_801903c0 = (s16)data_801903c0 - 1;
        if ((s16)data_801903c0 == 5) {
            func_800e5a28_slot0f(obj);
            return;
        }
    } else {
        data_801903c0 = 0;
    }
    one = 1;
    data_80190568 = one;
    r.x = 0x280;
    r.y = 0;
    r.w = 0x40;
    r.h = 0x100;
    func_80157fc4(&r, (u8 *)0x800b7000);
    func_80157d9c(0);
    r.x = 0x2c0;
    r.y = 0;
    r.w = 0x40;
    r.h = 0x100;
    func_80157fc4(&r, (u8 *)0x800bf000);
    func_80157d9c(0);
    func_800e5470_slot0f(obj);
    func_800e51c8_slot0f(0);
    func_800e5258_slot0f();
    func_800e5608_slot0f(obj);
    func_800e50a8_slot0f(obj);
    func_80137220(0, 6);
    b = func_8011f1e0();
    if (b != 0) {
        b->field_03 = 3;
        b->field_00 = one;
        b->field_02 = 0x99;
    }
    func_80151020(0x606);
}
