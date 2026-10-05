/* Reconstruction. Names/roles inferred, not original symbols. */
/* func_80156bc8: the unused 16-byte local reproduces the original 0x28 frame. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80156bc8(GameState *state, Object *object) {
    TextBuf *text;
    u8 unused[16];
    int i;

    if (object->field_120 + object->field_121 < 12) {
        if ((u8)(object->field_aa - 2) < 2) {
            if (object->field_aa != 3 || (state->field_33 & 1)) {
                text = &data_801817d0;
                if (object->side == 0) {
                    text = &data_801817c0;
                }
                for (i = 0; i < 4; i++) {
                    u8 byte = ((u8 *)object + i)[0x11c];
                    text->buf[i] = byte;
                }
                func_801519b4(text);
            }
        }
    }
}
