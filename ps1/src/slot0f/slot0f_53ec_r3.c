/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern u16 data_801903ca;
extern u8 data_801903c4[];
extern void (*data_800ebc68_slot0f[])(Object *);
void func_800e5b30_slot0f(int flag);
void func_8011abe4(void);
void func_8012818c(void);
void func_800e585c_slot0f(Object *obj);
void func_800e5874_slot0f(Object *obj);

void func_800e5608_slot0f(Object *obj) {
    func_800e5b30_slot0f(0);
}

void func_800e5628_slot0f(Object *obj) {
    u16 *timer = &data_801903ca;
    HudState *hud;
    *timer = *timer - 1;
    hud = data_8018f5a0;
    data_800ebc68_slot0f[hud->field_4c](obj);
    func_80138164();
    func_8011abe4();
    func_800e5874_slot0f(obj);
}

void func_800e56a8_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    if (data_801903c4[0] == 1) {
        data_8018f5a0->field_4c++;
        if (obj->field_ee != 0) {
            func_800e585c_slot0f(o);
        } else {
            func_8012818c();
        }
    }
}
