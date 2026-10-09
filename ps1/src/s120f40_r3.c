/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80121680(GameState *state, Object *a1, Object *a2) {
    a1->other = &player_left + 1;
    a2->other = &player_left;
    a1->field_0d = 0x10;
    a2->field_0d = 0xb;
    state->field_9c = &player_left;
    state->field_a0 = &player_left + 1;
    if (a1->field_cd != 0) {
        a1->field_d4 = 0;
        if (a1->kind == a2->kind && a2->field_d4 == 0) {
            a1->field_d4 = 1;
        }
    }
    if (a2->field_cd != 0) {
        a2->field_d4 = 0;
        if (a1->kind == a2->kind && a1->field_d4 == 0) {
            a2->field_d4 = 1;
        }
    }
}

void func_80121730(GameState *state) {
    if (data_8018f5a0->field_4e == 0) {
        func_80121780((Object *)state);
    } else if (data_8018f5a0->field_4e == 1) {
        func_801217d4((Object *)state);
    }
}

void func_80121780(Object *object) {
    data_8018f5a0->field_4e++;
    object->field_ab = 0;
    func_801285e0();
    func_801e1d0c(0);
    object->field_06 = 0xff;
    object->field_65 = 0xff;
}

void func_801217d4(Object *object) {
    u8 *flag = &data_801ae02c;
    table_8016e774[data_8018f5a0->field_50](object);
    if (*flag != 0) {
        func_801e1b64();
        func_801e1d6c(flag);
    }
    func_80138164();
    func_801510bc();
    func_80135ef0(object);
    func_8011a784();
}

void func_80121874(Object *object) {
    data_8018f5a0->field_50++;
    object->field_dc |= 0xe;
    if (object->field_af != 0) {
        data_8018f5a0->field_50 = 0xc;
        data_8018f5a0->field_60 = 0x10;
        object->field_09 = 1;
        object->field_2c = 0xff;
        func_8013245c();
        object->field_ab = 0;
        player_left.field_01 = 0;
        player_right.field_01 = 0;
    }
}
