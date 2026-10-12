/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80135858(void) {
    if ((game_state.versus_flag != 0 && *(u16 *)data_801a6984 == 0) || (game_state.round_time & 0x80) != 0) {
        Cell56 *base = data_80188d6c;
        Cell56 *e = &base[data_801a27d0];
        e->field_19 = 0xe0;
        e->tex_u = 0x20;
        base = (Cell56 *)((u8 *)base + 0x1c);
        e = &base[data_801a27d0];
        e->field_19 = 0xe0;
        e->tex_u = 0x30;
    } else {
        set_digits_from_number(game_state.round_time);
        data_80188d6c[data_801a27d0].tex_u = game_state.digits[1] << 4;
        data_80188d6c[data_801a27d0].digit_tex_u = game_state.digits[0] << 4;
        if (game_state.round_time == 0x63) {
            data_80188d6c[0].tex_param = data_80188ebc;
            data_80188d6c[0].tex_param_2 = data_80188ebc;
        } else if (game_state.round_time < 0x15) {
            u16 v;
            if ((game_state.frame_counter & 3) == 0) {
                v = data_80188ec0;
            } else {
                v = data_80188ebc;
            }
            data_80188d6c[0].tex_param = v;
            data_80188d6c[0].tex_param_2 = v;
            data_80188d6c[1].tex_param = v;
            data_80188d6c[1].tex_param_2 = v;
        }
    }
}
