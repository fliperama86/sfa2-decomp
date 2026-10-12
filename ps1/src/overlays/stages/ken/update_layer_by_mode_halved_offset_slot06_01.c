/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u16 data_80190542;

void update_layer_by_mode_halved_offset_slot06_01(Slot06Layer *layer) {
    Slot06Layer *l2;
    s16 d;

    if (layer->mode == 0) {
        init_sprite_counted((Sprite *)layer, 0xc0, 0);
    } else if (layer->mode == 1) {
        if (game_state.field_65 == 0) {
            *(s32 *)&layer->pos_x += -0x8000;
        }
        l2 = (Slot06Layer *)data_801aa5d4;
        d = l2->field_22;
        d -= l2->ref_x;
        d += data_80190542;
        d /= 2;
        d += layer->ref_x;
        d += layer->pos_x_int;
        layer->field_22 = d;
        d = l2->field_26;
        d -= l2->field_0e;
        d /= 2;
        d += layer->field_0e;
        d += layer->pos_y_int;
        layer->field_26 = d;
    }
}
