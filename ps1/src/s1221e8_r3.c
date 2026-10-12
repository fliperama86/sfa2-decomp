/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801229b4(Entity *entity) {
    u8 s0;
    u16 v;
    if (entity->countdown != 0) {
        s0 = ref_first.p->field_255 + 2;
        v = func_80147158(ref_first.p);
        ref_first.p->effect_code_bc = s0;
        func_80155eac(v, ref_first.p->team_id);
        func_80155d4c(v, ref_first.p->team_id);
    }
}

void func_80122a44(GameState *state) {
    table_8016e7d0[((HudBig *)data_8018f5a0)->state_index](state);
    func_8012546c();
}

void func_80122a94(GameState *state) {
    ((HudBig *)data_8018f5a0)->state_index++;
    ((Entity *)state)->field_116 = 0;
    ((Entity *)state)->countdown_timer_2 = 0x32;
    if (((Entity *)state)->countdown != 0) {
        func_80123b20(((Entity *)state)->field_7c, (Entity *)state);
    } else if (((Entity *)state)->field_4d == 0 && ((Entity *)state)->nonzero_flag != 0) {
        ((Entity *)state)->countdown_timer_2 = 0x14;
    }
    if (((Entity *)state)->field_78 != 0) {
        ((Entity *)state)->field_88 = ((Entity *)state)->field_78->team_id;
    }
    func_80122b3c(state);
}

void func_80122b3c(GameState *state) {
    if (((Entity *)state)->countdown != 0) {
        ((Entity *)state)->field_11a = 0xff;
        ((Entity *)state)->countdown--;
    } else {
        ((HudBig *)data_8018f5a0)->state_index++;
        ((Entity *)state)->field_6c = 5;
        if (((Entity *)state)->field_11a != 0) {
            func_8014f4d4(5, 1);
        }
    }
}

void func_80122bb0(GameState *state) {
    u16 v;
    if (((Entity *)state)->table_compare_value != table_8016e7e0[((Entity *)state)->table_index]) {
        func_80122c5c((Entity *)state);
    } else {
        ((HudBig *)data_8018f5a0)->state_index++;
        if (((Entity *)state)->mode_flag_b != 0) {
            ((Entity *)state)->countdown_timer = 0xa0;
        } else {
            ((Entity *)state)->countdown_timer = 0x5a;
        }
        if (((Entity *)state)->field_4d == 0 && ((Entity *)state)->nonzero_flag == 0) {
            func_80155f30(((Entity *)state)->field_7c);
        }
    }
}
