/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

u8 update_script_block_skip_counter(Object *object) {
    u8 t;
    if (data_80189468 == 0) {
        object->field_224++;
        call_script_subop_from_next_word();
        return 1;
    }
    if (data_80189468 == 2) {
        if (object->field_224 == data_8018946c) {
            goto zero;
        }
        return 1;
    }
    if (data_80189468 == 4) {
        t = object->field_224 - 1;
        object->field_224 = t;
        if ((u8) (data_8018946c - t) != 1) {
            return 1;
        }
    }
zero:
    return 0;
}
