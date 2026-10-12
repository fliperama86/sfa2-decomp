/* Reconstruction. Names/roles inferred, not original symbols. */
/* queue_name_text: the unused 16-byte local reproduces the original 0x28 frame. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void queue_name_text(GameState *state, Object *object) {
    TextBuf *text;
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[16];
    int i;

    if (object->score_rank + object->field_121 < 12) {
        if ((u8)(object->phase_index - 2) < 2) {
            if (object->phase_index != 3 || (state->loop_counter & 1)) {
                text = &data_801817d0;
                if (object->side == 0) {
                    text = &data_801817c0;
                }
                for (i = 0; i < 4; i++) {
                    u8 byte = ((u8 *)object + i)[0x11c];
                    text->buf[i] = byte;
                }
                add_item_to_table(text);
            }
        }
    }
}
