/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


extern s8 data_801a6984;


void func_80128c48(void) {
    func_80128c80(&player_left);
    func_80128c80(&player_left + 1);
}

void func_80128c80(Object *object) {
    s16 delta, magnitude, negative;
    if (object->field_7e == 0 && object->field_299 == 0 &&
        object->field_49 == 0) {
        negative = 0;
        delta = (u16)object->pos_x - (u16)object->other->pos_x;
        magnitude = delta;
        if (delta < 0) {
            negative = 1;
            magnitude = -delta;
        }
        if ((s16)(magnitude + 0x18) > 0x30) {
            object->field_158 = 0;
            if (negative) object->field_158 = 1;
        }
    }
}

void func_80128d08(void) {
    if (player_left.frame->active == 0) data_801985b8 = 0;
    if (player_right.frame->active == 0) data_80198225 = 0;
}

void func_80128d58(void) {
    func_80128da4(&player_left, &player_left + 1);
    func_80128da4(&player_left + 1, &player_left);
}

void func_80128da4(Object *a, Object *b) {
    int x, y;
    x = b->frame->field_07;
    y = 0;
    if (x != 0) {
        Box6 *box = &((Box6 *)b->unknown_148)[x];
        int extent = box->extent;
        x = box->origin;
        y = -extent;
        if (a->field_158 != 1) {
            x = -x;
            y = extent;
        }
    }
    y = a->pos_x - (b->pos_x + x + y);
    if (y < 0) y = -y;
    a->field_21e = y;
}

void func_80128e1c(void) {
    func_80128edc(&player_right);
    func_80128edc(&player_right - 1);
}

void func_80128e54(void) {
    func_80128edc(&player_left);
}

void func_80128e7c(void) {
    func_80128edc(&player_left);
    func_80128edc(&player_left + 1);
}

void func_80128eb4(void) {
    func_80128edc(&player_right);
}

void func_80128edc(Object *object) {
    if (object->field_249) object->field_249--;
    if (object->field_cd == 0) {
        if (game_state.field_30 != 0 && data_801a6984 != 0 &&
            game_state.mode == object->side + 1)
            func_80131c7c(object);
        func_80131854(object);
        func_8013172c(object);
    }
    if (object->field_15d) {
        object->field_15d--;
        if (object->field_15d == 0) object->field_15e = 0;
    }
    func_80128fc4(object);
}

void func_80128fc4(Object *object) {
    handlers_04[object->field_04](object);
    func_801321e8(object);
}

void func_80129018(Object *unused) {
}
