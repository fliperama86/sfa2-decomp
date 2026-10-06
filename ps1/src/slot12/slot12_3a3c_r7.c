/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
void func_8011abe4(void);
void func_8012818c(void);
extern void (*data_8002445c_slot12[])(Object *);
void func_80014268_slot12(Object *obj);

void func_8001405c_slot12(Object *obj) {
    game_state.field_2c2 = game_state.field_2c2 - 1;
    data_8002445c_slot12[data_8018f5a0->field_4e](obj);
    func_80138164();
    func_8011abe4();
}

void func_800140c8_slot12(Object *o) {
    Slot12Obj *obj = (Slot12Obj *)o;
    if (game_state.field_2bc == 1) {
        data_8018f5a0->field_4e = data_8018f5a0->field_4e + 1;
        if (obj->field_ee != 0) {
            func_80014268_slot12(o);
        } else {
            func_8012818c();
        }
    }
}
