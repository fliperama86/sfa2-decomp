/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_8012fe60(Object *object) {
    object->field_0b = object->field_158;
    if (game_state.field_30 != 0 && game_state.mode != object->side + 1) {
        ref_other.p = object->other;
        if (object->field_1a0 != 0 && ref_other.p->field_157 != 0) {
            object->field_157 = 1;
            object->field_04 = 1;
            object->field_06 = 6;
            object->field_05 = 0;
            object->field_07 = 2;
            func_80130678(object, 0x18);
            return;
        }
    }
    if (object->field_29f == 0 && (object->field_130 & 0x4000)) {
        object->field_157 = 1;
        object->field_04 = 1;
        object->field_06 = 6;
        object->field_05 = 0;
        object->field_07 = 2;
        func_80130678(object, 0x18);
    } else {
        object->field_04 = 1;
        object->field_157 = 0;
        object->field_05 = 0;
        object->field_06 = 6;
        object->field_07 = 0;
        func_80130678(object, 0x15);
    }
}

void func_8012ff80(Object *object) {
    u8 v;
    s16 hp;
    int index;
    game_state.field_358 = object->other;
    hp = object->field_5c;
    if ((s16)game_state.field_358->field_5c >= hp) {
        index = 0;
        object->field_04 = 1;
        object->field_05 = 5;
    } else {
        v = 4;
        if (hp == 0x90) {
            object->field_ee = object->field_ee + 1;
            v = 0x84;
        }
        index = 0;
        object->field_167 = v;
        *((u8 *)object + object->field_ce + 0xcf) = v;
        object->field_04 = 1;
        object->field_05 = 4;
    }
    object->field_06 = 0;
    object->field_07 = 0;
    object->field_7e = 0;
    object->field_2a2 = 0;
    object->field_2a1 = 0;
    object->field_182 = 0;
    object->field_180 = 0;
    func_80130678(object, index);
}

void func_80130038(Object *object) {
    s32 speed;
    int held = object->field_130 & 0x8000;
    object->field_48 = held == 0;
    speed = motion_speed_table[object->kind * 2 + object->field_48];
    if (object->field_0b != 0) speed = -speed;
    object->field_4c = speed;
}
