/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80130678(Object *object, int arg);

void func_8012f4dc(Object *object) {
    object->field_04 = 1;
    object->field_05 = 6;
    object->field_06 = 0;
    object->field_07 = 0;
    object->field_259 = 0;
    object->field_258 = 0;
    object->field_25a = 0;
    func_8013786c(object);
    game_state.field_358 = object->other;
    if (game_state.field_358->field_167 < 0xb) {
        func_80155f30(object);
    }
    func_801308c4(object, 0x1b);
}

unsigned char func_8012f56c(Object *object) {
    game_state.field_358 = object->other;
    return *(u16 *)&game_state.field_358->field_04 == 0x601;
}

void func_8012f59c(Object *object) {
    object->field_04 = 1;
    object->field_05 = 4;
    object->field_06 = 0;
    object->field_07 = 0;
    if ((s16)object->field_5c == 0x90) {
        u8 count;

        count = object->field_ee + 1;
        object->field_167 |= 0x80;
        object->field_ee = count;
    }
    ((u8 *)object + object->field_ce)[0xcf] = object->field_167;
}

void func_8012f5ec(Object *object) {
    if (object->field_251 != 0 && object->field_d8 != 0) {
        object->field_251 = 0;
    }
}

int func_8012f618(Object *object) {
    unsigned char result;

    result = 0;
    if (game_state.field_30 != 0 && game_state.mode != object->side + 1) {
        ref_other.p = object->other;
        if (object->field_1a0 != 0) {
            result = ref_other.p->field_157 != 0;
        }
    }
    if (object->field_130 & 0x4000) {
        result = 1;
    }
    return result;
}

void func_8012f6a0(Object *object) {
    object->field_04 = 1;
    object->field_05 = 0;
    object->field_06 = 1;
    object->field_07 = 0;
    object->field_157 = 0;
    func_80130678(object, 7);
}

int func_8012f6d8(Object *object) {
    int bits;

    if (game_state.field_30 != 0 && game_state.mode != object->side + 1) {
        ref_other.p = object->other;
        if (object->field_1a0 != 0 && ref_other.p->field_157 != 0 &&
            object->field_15b == 0) {
            return 1;
        }
    }
    if (object->field_29f != 0) {
        bits = object->field_134 & 0x4000;
    } else {
        bits = object->field_130 & 0x4000;
    }
    return bits != 0;
}

void func_8012f784(Object *object) {
    object->field_04 = 1;
    object->field_06 = 1;
    object->field_05 = 0;
    object->field_07 = 2;
    object->field_157 = 0;
    func_80130678(object, 4);
}

int func_8012f7c0(Object *object) {
    int result;
    u16 bits;

    result = 0;
    bits = object->field_130 & 0xa000;
    if (object->field_7e == 0) {
        if (bits == 0x8000 || bits == 0x2000) {
            result = 1;
        }
    }
    return result;
}

int func_8012f800(Object *object) {
    int result;
    u16 flags;

    flags = object->field_130;
    result = 0;
    if (object->field_7e == 0) {
        if (flags == 0x8000 || flags == 0x2000) {
            result = 1;
        }
    }
    return result;
}

void func_8012f838(Object *object) {
    int arg;

    object->field_04 = 1;
    object->field_05 = 0;
    object->field_06 = 2;
    object->field_07 = 1;
    func_80130038(object);
    if (object->field_130 & 0x8000) {
        arg = 2;
    } else {
        arg = 3;
    }
    func_80130678(object, arg);
}
