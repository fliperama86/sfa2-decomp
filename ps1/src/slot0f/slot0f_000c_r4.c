/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
void func_800e05a0_slot0f(int a, int b);
void func_800e0640_slot0f(GameState *g);

void func_800e0400_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    HudState *h = data_8018f5a0;
    h->field_4a += 1;
    h->field_4c = 0;
    obj->field_c6 = 0x1e0;
    func_80138358((GameState *)obj);
    func_8013839c((GameState *)obj);
    obj->field_2d = 7;
    player_left.field_cf = 0x1f;
    player_right.field_cf = 0x1f;
    player_left.field_20b = (func_80151184() & 3) << 1;
    player_right.field_20b = (func_80151184() & 3) << 1;
    func_800e05a0_slot0f(1, 0);
}

void func_800e04a8_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    s16 t;
    func_800e05a0_slot0f(1, 1);
    t = obj->field_c6;
    t -= 1;
    obj->field_c6 = t;
    if (t == 0) {
        data_8018f5a0->field_4a += 1;
        player_right.field_01 = 0;
        player_left.field_01 = 0;
        obj->field_4e = 1;
    } else {
        u8 c;
        c = obj->field_4a - 1;
        obj->field_4a = c;
        if (c == 0) {
            obj->field_4a = 0x3b;
            obj->field_49--;
        }
        func_800e0640_slot0f((GameState *)o);
    }
}
