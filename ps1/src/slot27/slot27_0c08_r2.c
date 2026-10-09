/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800111c8_slot27(void);

void func_80010d10_slot27(void) {
    u16 *c = &game_state.field_c6;
    int t = *c - 1;
    *c = t;
    if ((s16)t == 0) {
        func_8011eae4();
        func_800111c8_slot27();
        data_8018f5a0->field_4c = 2;
        data_8018f5a0->field_4e = 0;
        data_8018f5a0->field_50 = 0;
        data_8018f5a0->field_52 = 0;
        data_8018f5a0->field_54 = 0;
    }
}

void func_80010d7c_slot27(void) {
    Object *p;
    u8 k;
    p = &player_left;
    k = player_left.kind;
    if (game_state.field_1c != 0 || player_left.field_118 != k) {
        player_left.field_ef = 1;
    }
    if (p->field_cd == 0) {
        p->field_118 = k;
        p->field_11a = p->field_d8;
        p->field_119 = p->field_d4;
    }
    p = &player_right;
    k = player_right.kind;
    if (game_state.field_1c != 0 || player_right.field_118 != k) {
        player_right.field_ef = 1;
    }
    if (p->field_cd == 0) {
        p->field_118 = k;
        p->field_11a = p->field_d8;
        p->field_119 = p->field_d4;
    }
}
