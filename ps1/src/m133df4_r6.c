/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u16 data_801a27e4[];

void func_801378d8(Object *object) {
    Rect r;
    u8 i, j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 0x10; j++) {
            data_801a27e4[((object->field_0d + i) << 4) + j] =
                (data_801a27e4 + 0xa00)[((object->field_0d + i) << 4) + j];
        }
    }
    r.x = 0x60;
    r.y = object->field_0d + 0x1e0;
    r.w = 0x10;
    r.h = 5;
    func_80157fc4(&r, (u8 *)data_801a27e4 + 0x1400 + (object->field_0d << 5));
    data_8018daf4[object->side] = 0;
}
