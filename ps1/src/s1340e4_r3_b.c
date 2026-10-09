/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80136290(void) {
    data_80172410[game_state.field_40](&data_801aa5d4);
    data_801aa5fe = data_801aa5f6;
    data_801aa602 = data_801aa5fa;
}

void func_801362f4(Sprite *sprite) {
    func_80136404(sprite, 0x1c0, 0);
    if (sprite->pending != 0) {
        sprite->pending = 0;
        sprite->count++;
        if (game_state.field_0a == 6) sprite->field_4a = 0x200;
    }
}

void func_8013635c(Sprite *sprite) {
    func_80136404(sprite, 0x1c0, 0);
    if (sprite->pending != 0) {
        sprite->pending = 0;
        sprite->count++;
    }
}

void func_801363ac(Sprite *sprite) {
    if (sprite->field_00 != 0 && game_state.field_4b == 0) {
        func_801364a0(sprite);
        func_80136744(sprite);
    }
}
