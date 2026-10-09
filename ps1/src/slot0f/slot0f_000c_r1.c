/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800e000c_slot0f(GameState *state, Menu *menu) {
    func_801192bc(0x3c);
    scratch_word_00 = -1;
    scratch_word_04 = 1;
    func_80150cd0(2);
}

void func_800e004c_slot0f(Object *obj) {
    u8 t = obj->field_02;
    obj->field_02 = 0;
    obj->field_03 = t;
    if (data_801a89a8[0] == 0) {
        obj->field_02 = 1;
    }
    if (data_801a89a8[4] == 0) {
        obj->field_02 |= 2;
    }
}

void func_800e009c_slot0f(Object *obj, int arg) {
    HudState *h = data_8018f5a0;
    h->field_48 = 0;
    h->field_4a = 0;
    h->field_4c = 0;
    h->field_4e = 0;
    h->field_50 = 0;
    h->field_52 = 0;
    obj->field_c2 = 0;
    obj->field_c4 = 0;
    obj->field_c6 = 0;
    obj->field_c8 = 0;
    obj->field_ca = 0;
    *(u16 *)&obj->field_cc = 0;
    obj->field_01 = 0;
    *(u8 *)&obj->pos_y = 1;
    player_left.side = 0;
    player_right.side = 1;
    func_80119340(1);
    if (arg == 0) {
        func_80119144(1, 5);
        func_80119198(1);
    } else if (arg == 1) {
        func_80119198(3);
    } else {
        func_80119198(1);
    }
}
