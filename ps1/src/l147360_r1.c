/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80147360(Object *object) {
    u8 a;
    u8 b;
    advance_sequence_unless_paused(object);
    if (game_state.field_64 != 0) {
        if (*(u8 *)&object->step_flags != 0) return;
        game_state.field_dc = 0;
        if (data_8018945c != 0) {
            int v = object->active_state >> 1;
            if (v == 0) {
                a = 0;
                b = 0;
            } else if (v == 1) {
                a = 0;
                b = 0;
            } else if (v == 2) {
                a = 0;
                b = 0x10;
            } else if (v == 3) {
                a = 1;
                b = 0x10;
            }
            store_image_row_and_queue_job(object, a, data_80189458 + b);
            data_80189458 = data_80189458 + 1;
            if (data_80189458 == 0x10) data_80189458 = 0;
        }
    } else {
        game_state.field_dc = data_80189454;
        object->state_0 = object->state_0 + 1;
    }
    data_8018945c = (data_8018945c + 1) & 1;
}
