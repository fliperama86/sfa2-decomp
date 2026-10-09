/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_8012f898(Object *object);

void func_801312b8(Object *object) {
    func_80131f7c(object);
    func_80130678(object, 0);
    object->field_04 = 1;
    object->field_05 = 0;
    object->field_06 = 0;
    object->field_07 = 0;
    object->field_157 = 0;
    func_8013151c(object);
    func_80155f30(object);
    object->field_170 = 0;
    object->field_6a = 0;
    if (game_state.field_30 != 0 && game_state.mode != object->side + 1) {
        object->field_1a1 = object->field_1a1 - 1;
        if (object->field_1a1 & 0x80) {
            object->field_19f = 0;
        }
    }
    object->field_69 = 0;
}

void func_8013136c(Object *object) {
    if (object->field_cd == 0 && object->kind == 10) {
        func_801313bc(object, 0x30);
    } else {
        func_801312b8(object);
    }
}

void func_801313bc(Object *object, unsigned short arg) {
    func_80130678(object, arg);
    object->field_04 = 1;
    object->field_05 = 0;
    object->field_06 = 0;
    object->field_07 = 0;
    object->field_157 = 0;
    func_8013151c(object);
    func_80155f30(object);
    object->field_170 = 0;
    object->field_6a = 0;
    if (game_state.field_30 != 0 && game_state.mode != object->side + 1) {
        object->field_1a1 = object->field_1a1 - 1;
        if (object->field_1a1 & 0x80) {
            object->field_19f = 0;
        }
    }
    object->field_69 = 0;
}

void func_80131468(Object *object) {
    func_80131f7c(object);
    object->field_04 = 1;
    object->field_05 = 0;
    object->field_06 = 0;
    object->field_07 = 2;
    object->field_157 = 1;
    func_8013151c(object);
    func_80155f30(object);
    object->field_170 = 0;
    object->field_6a = 0;
    if (game_state.field_30 != 0 && game_state.mode != object->side + 1) {
        object->field_1a1 = object->field_1a1 - 1;
        if (object->field_1a1 & 0x80) {
            object->field_19f = 0;
        }
    }
    object->field_69 = 0;
    func_80130678(object, 1);
}

void func_8013151c(Object *object) {
    object->field_45 = 0;
    object->field_15b = 0;
    object->field_15c = 0;
    object->field_165 = 0;
    object->field_166 = 0;
    object->field_241 = 0;
    object->field_247 = 0;
    object->field_248 = 0;
    object->field_24e = 0;
    object->field_251 = 0;
    object->field_254 = 0;
    object->field_256 = 0;
    object->field_258 = 0;
    object->field_259 = 0;
    object->field_25a = 0;
    object->field_25b = 0;
    object->field_252 = 0;
    object->field_25f = 0;
    object->field_260 = 0;
    object->field_17b = 0;
    object->field_16a = 0;
    object->field_288 = 0;
    object->field_289 = 0;
    object->field_17c = 0xff;
    object->field_17d = 0xff;
    object->field_17e = 0xff;
    object->field_28a = 0;
    object->flags_28b = 0;
    object->field_28f = 0;
    object->pos_y = object->field_70;
    *(unsigned int *)&object->field_14 &= 0xffff0000;
    object->field_295 = 0;
    object->field_0b = object->field_158;
    object->field_49 = 0;
    object->field_4b = 0;
    object->field_299 = 0;
    object->field_29c = 0;
    object->field_29d = 0;
    object->field_278 = 0;
    object->field_279 = 0;
    object->field_217 = 0;
    object->field_218 = 0;
    object->field_219 = 0;
    object->field_221 = 0;
    object->field_242 = 0;
    object->field_29a = 0;
    object->field_2a3 = 0;
    if (object->field_7e == 0) {
        object->field_159 = 0;
    }
    game_state.field_358 = object->other;
    if (game_state.field_358->field_7e == 0) {
        object->field_225 = 0;
        object->field_216 = 0;
    }
}

void func_80131638(Object *object) {
    func_80131f7c(object);
    object->field_04 = 1;
    object->field_06 = 3;
    object->field_05 = 0;
    object->field_07 = 2;
    object->pos_y = object->field_70;
    func_8013151c(object);
    if (object->field_cd == 0) {
        if (func_80130470(object)) {
            func_8013047c(object);
        } else if (func_8012f970(object)) {
            func_8012fe60(object);
        } else if ((u8)func_8012f898(object)) {
            func_8012f8c4(object);
        } else {
            func_80130678(object, 0x11);
        }
    } else {
        if ((u8)func_8012f898(object)) {
            func_8012f8c4(object);
        } else {
            func_80130678(object, 0x11);
        }
    }
}
