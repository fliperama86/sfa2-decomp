/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80135858(void) {
    if ((game_state.field_30 != 0 && *(u16 *)data_801a6984 == 0) || (game_state.field_49 & 0x80) != 0) {
        Cell56 *base = data_80188d6c;
        Cell56 *e = &base[data_801a27d0];
        e->field_19 = 0xe0;
        e->field_18 = 0x20;
        base = (Cell56 *)((u8 *)base + 0x1c);
        e = &base[data_801a27d0];
        e->field_19 = 0xe0;
        e->field_18 = 0x30;
    } else {
        func_8011fec8(game_state.field_49);
        data_80188d6c[data_801a27d0].field_18 = game_state.digits[1] << 4;
        data_80188d6c[data_801a27d0].field_34 = game_state.digits[0] << 4;
        if (game_state.field_49 == 0x63) {
            data_80188d6c[0].field_1a = data_80188ebc;
            data_80188d6c[0].field_36 = data_80188ebc;
        } else if (game_state.field_49 < 0x15) {
            u16 v;
            if ((game_state.field_1d & 3) == 0) {
                v = data_80188ec0;
            } else {
                v = data_80188ebc;
            }
            data_80188d6c[0].field_1a = v;
            data_80188d6c[0].field_36 = v;
            data_80188d6c[1].field_1a = v;
            data_80188d6c[1].field_36 = v;
        }
    }
}
