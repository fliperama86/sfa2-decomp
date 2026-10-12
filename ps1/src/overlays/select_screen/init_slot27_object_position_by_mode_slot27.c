/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_80028b88_slot27)(Object *);

void init_slot27_object_position_by_mode_slot27(Object *obj) {
    obj->ot_depth = 1;
    obj->field_0c = 1;
    obj->image_row = 0x16;
    obj->pos_x = 0x320;
    obj->pos_y = 0x54;
    obj->variant_index = 0;
    obj->countdown = 0x1f;
    obj->state_0++;
    if ((game_state.mode & 1) == 0) {
        obj->variant_index = 1;
        obj->pos_x = 0x2e0;
    }
    data_80028b88_slot27(obj);
}
