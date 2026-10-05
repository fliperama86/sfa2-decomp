/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_8013600c(void) {
    func_80136290();
    func_8013606c();
    func_801368fc();
}

void func_8013603c(void) {
    func_80136290();
    func_801368fc();
    func_8013606c();
}

void func_8013606c(void) {
    u16 p, q;
    data_801723c0[game_state.field_40](&data_801aa544);
    p = data_801aa566;
    q = data_801aa56a;
    data_801aa556 = p;
    data_801aa55a = q;
    data_801aa594 = data_801aa598;
    data_801aa594 += data_801aa58c * 2;
    data_801aa594 += data_801aa58e * 2;
    data_801aa56e = p;
    data_801aa572 = q;
}

void func_80136120(Sprite *sprite) {
    func_801361fc(sprite, 0x1c0, (game_state.field_0a == 6) << 8);
    if (sprite->pending != 0) {
        sprite->pending = 0;
        sprite->count++;
    }
}

void func_80136180(Sprite *sprite) {
    func_801361a4(sprite, 0x1c0, 0);
}

void func_801361a4(Sprite *sprite, s16 x, s16 y) {
    func_801361fc(sprite, x, y);
    if (sprite->pending != 0) {
        sprite->pending = 0;
        sprite->count++;
    }
}
