/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8011fec8(int n) {
    GameState *g = &game_state;
    int i;
    for (i = 0; i < 16; i++) {
        g->digits[i] = n % 10;
        n = n / 10;
    }
}

void func_8011ff24(void) {
    data_801ad354 = data_8018f5e0;
    data_8018db00 = data_8019036c;
    data_8018db04 = data_801903ac;
    data_80190104 = 0;
    data_80190100 = 0;
    data_8018f59c = 0;
}

void func_8011ff74(Object *object) {
    int margin = box_margin[0];
    if ((u16)((u16)object->pos_x - margin + 0x40) > 0x200) {
        object->field_04 = 3;
    }
}
