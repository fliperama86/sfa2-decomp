/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

void func_801427d8(Object *object) {
    s16 mode = game_state.field_354;
    s16 limit = 0;
    int level;
    s16 c6;
    /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
    do {
        if (mode == 1 || mode == 2 || mode == 0x94 || mode == 0x68) {
            limit = 4;
        } else if (mode == 0x90 || mode == 0x84 || mode == 0x14 || mode == 0x60 ||
                   mode == 0x48 || mode == 0x28) {
            limit = 2;
        }
    } while (0);
    c6 = object->meter;
    level = 0;
    if (c6 >= 0x60) {
        level = 2;
        if (c6 >= 0x90)
            level = 4;
    }
    if (limit > level)
        limit = level;
    object->direction_index = limit;
    object->field_255 = limit;
    func_80142c04(object);
}
