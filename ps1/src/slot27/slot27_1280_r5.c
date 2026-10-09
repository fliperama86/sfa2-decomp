/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001188c_slot27(Object *obj);
extern HudState *data_8018f5a0;

void func_8001175c_slot27(Object *obj) {
    Object *p = obj->other;
    int v;
    int k;

    if (data_8018f5a0->field_4e >= 3 && game_state.mode != 3) {
        obj->field_04 = obj->field_04 + 1;
        v = 8;
        if (p->side == 0) {
            v = -8;
        }
        obj->field_4c = v;
    }
    k = p->field_d4;
    if (k != obj->field_45) {
        obj->field_45 = k;
        func_8001188c_slot27(obj);
    }
    func_80131094(obj);
}
