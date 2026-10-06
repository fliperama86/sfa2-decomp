/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
void func_801280f0(void);
Block172 *func_8011f1e0(void);
void func_80014288_slot12(Object *o);
void func_80013ed8_slot12(Object *obj);
void func_80013b5c_slot12(int a);
void func_80013bec_slot12(void);
void func_80013e04_slot12(Object *obj);
void func_80013a3c_slot12(Object *obj);

void func_80013708_slot12(Object *obj) {
    Rect rect;
    Object *p;
    HudState *h = data_8018f5a0;

    h->field_4c++;
    game_state.field_2bc = 0;
    h->field_4e = 0;
    game_state.field_2b8--;
    if (game_state.field_2b8 == 5) {
        func_80014288_slot12(obj);
    } else {
        func_801280f0();
        func_80151020(0x603);
        func_8011eb14();
        func_8011eae4();
        func_8011ee64();
        data_80190568 = 1;
        rect.x = 0x280;
        rect.y = 0;
        rect.w = 0x40;
        rect.h = 0x100;
        func_80157fc4(&rect, (u8 *)0x800b1000);
        func_80157d9c(0);
        rect.x = 0x2c0;
        rect.y = 0;
        rect.w = 0x40;
        rect.h = 0x100;
        func_80157fc4(&rect, (u8 *)0x800b9000);
        func_80157d9c(0);
        func_80013ed8_slot12(obj);
        func_80013b5c_slot12(0);
        func_80013bec_slot12();
        func_80013e04_slot12(obj);
        func_80013a3c_slot12(obj);
        func_80137220(0, 6);
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_03 = 3;
            p->field_00 = 1;
            p->field_02 = 0xa4;
        }
    }
}
