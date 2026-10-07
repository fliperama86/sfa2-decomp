/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"

extern TextObj *table_80180454[];
extern u8 *table_80180ecc[];
void func_801519b4(TextObj *t);

void func_80153d7c(Actor *a) {
    TextObj *t;
    u8 *src;
    int i;
    t = table_80180454[a->field_01];
    src = table_80180ecc[a->field_01];
    src = src + a->field_08 * 0x30;
    for (i = 0; i < 0x30; i++) {
        t->buf[i] = src[i];
    }
    func_801519b4(t);
}
