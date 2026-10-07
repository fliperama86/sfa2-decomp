/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
void func_800e483c_slot0f(void);
void func_80120498(int a);
void func_800e4988_slot0f(void);
void func_800e009c_slot0f(Object *obj, int a);

void func_800dfc6c_slot0f(GameState *state, Menu *menu) {
    if (((data_801a696a | data_801a6976) & 0x860) != 0) {
        if (((data_801a696a | data_801a6976) & 0x820) != 0) {
            data_8018f5a0->field_4c = 0;
            data_8018f5a0->field_4a++;
        } else {
            func_800e009c_slot0f((Object *)state, 1);
        }
        func_80120554(0, 0, 0x205);
        func_80120498(0x200);
    } else {
        state->field_c2 = (s16)state->field_c2 - 1;
        if ((s16)state->field_c2 == 0) {
            func_800e009c_slot0f((Object *)state, 1);
        }
    }
    func_800e4988_slot0f();
}
