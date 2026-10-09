/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"
/* field_b0 / field_b2 are used as signed 16-bit counters here (hence the s16 views); field_b2 also as a byte at 0xb2. */


extern HudWide *data_8018f5a0;
void func_801564b0(void *a, Object *object);
int func_80127cd8(u8 a, s16 b, u8 c);

void func_80126cd0(GameState *state, Object *object) {
    if (state->field_09 != 0 || state->field_2c != 0) {
        func_801269e0(object);
    } else {
        if (--*(s16 *)&object->field_b0 == 0) {
            object->field_b0 = 0x7f;
            if (*(s16 *)&object->field_b2 == 0) {
                func_80126d68(state, object);
                return;
            }
            (*(s16 *)&object->field_b2)--;
        }
        func_80135c80(object, 0xe);
    }
}

void func_80126d68(GameState *state, Object *object) {
    object->field_aa = 0;
    object->field_ab = 0;
    object->field_ac = 0;
    object->field_b0 = 0;
    object->field_b2 = 0;
    object->field_c1 = 0;
    object->field_f1 = 0;
    object->field_a9++;
    if (object->side == 0)
        state->field_08 &= 0xfe;
    else if (object->side == 1)
        state->field_08 &= 0xfd;
    func_80135c80(object, 0);
}

void func_80126de8(GameState *state, Object *object) {
    if (state->field_09 == 0 && state->field_2c == 0) {
        u16 pad;
        if (object->side != 0)
            pad = data_801a6976;
        else
            pad = data_801a696a;
        if (pad & 0xfc) {
            if (*(s16 *)&object->field_b2 == 0) {
                object->field_b0 = 1;
            } else {
                (*(s16 *)&object->field_b2)--;
                object->field_b0 = 0x60;
            }
        }
    }
}

u8 func_80126e74(GameState *state, Object *object) {
    return func_80126e98(state, object);
}

u8 func_80126e98(GameState *state, Object *object) {
    if (state->field_46 == 0 && state->field_09 == 0) {
        u16 pad;
        if (object->side == 0)
            pad = data_801a696a;
        else
            pad = data_801a6976;
        if (pad & 0x800)
            return 1;
    }
    return 0;
}

void func_80126efc(GameState *state, Object *object) {
    if (state->field_2c == 0) {
        TextBuf *buf = table_8016f5f4[object->side];
        *buf->buf = *(u8 *)&object->field_b2 + 0x30;
        func_801519b4(buf);
    }
}

void func_80126f54(GameState *state, Object *object) {
    if (state->field_2c != 0)
        func_801269e0(object);
    else
        func_801564b0(state, object);
}

void func_80126f94(GameState *state, Object *object) {
    if (state->field_46 != 0)
        func_801269e0(object);
    else
        table_8016f5bc[object->field_aa]();
}

void func_80126ff4(GameState *state, Object *object) {
    object->field_b0 = 0x5a;
    object->field_aa++;
    func_80135c80(object, 0xf);
}

void func_8012702c(GameState *state, Object *object) {
    if ((~object->field_132 & object->field_130 & 0xfc) == 0 && --*(s16 *)&object->field_b0 != 0) {
        func_80135c80(object, 0xf);
    } else {
        object->field_b0 = 0x1e;
        object->field_aa++;
    }
}

void func_801270a4(GameState *state, Object *object) {
    if (--*(s16 *)&object->field_b0 == 0) {
        object->field_b8 = 0;
        object->field_e0 = 0;
        object->field_e4 = 0;
        object->field_c1 = 0;
        object->field_aa++;
        if (object->side == 0)
            state->field_17 &= 0xfe;
        else if (object->side == 1)
            state->field_17 &= 0xfd;
        func_80135c80(object, 0);
    } else {
        func_80135c80(object, 0xf);
    }
}

void func_80127140(GameState *state, Object *object) {
    if (state->field_27 != 0) {
        func_801269e0(object);
    } else {
        object->field_a9 = 0;
        object->field_aa = 0;
        object->field_ab = 0;
        object->field_ac = 0;
    }
}

void func_80127188(void) {
    game_state.field_ee = 0x1f;
    do {
        data_8018f5a0->field_48 = 0;
        table_8016f5fc[data_8018f5a0->field_68 >> 1]();
        func_801192bc(data_8018f5a0->field_69);
    } while (game_state.field_ee != 0);
    game_state.field_f0 = 0;
    func_801192f0();
}

void func_80127228(void) {
    game_state.field_ec = 0;
    func_80127510();
    func_801275ac();
    if (game_state.field_ec == 0xc)
        game_state.field_ee = 0;
}

void func_80127278(void) {
    game_state.field_ec = 0;
    func_801273d8();
    func_80127510();
    func_801275ac();
    if (game_state.field_ec == 0xd)
        game_state.field_ee = 0;
}

void func_801272d0(void) {
    game_state.field_ec = 0;
    func_8012774c();
    func_80127884();
    func_80127920();
    if (game_state.field_ec == 0xd)
        game_state.field_ee = 0;
}

void func_80127328(void) {
    game_state.field_ec = 0;
    func_801273d8();
    func_80127474();
    func_80127510();
    func_801275ac();
    func_80127648();
    if (game_state.field_ec == 0x1f)
        game_state.field_ee = 0;
}

void func_80127390(void) {
    game_state.field_ec = 0;
    func_801273d8();
    if (game_state.field_ec == 1)
        game_state.field_ee = 0;
}

void func_801273d8(void) {
    if ((u8)func_80127cd8(0, -1, 0))
        game_state.field_ec |= 1;
    func_80137220(0, 6);
}

void func_8012742c(void) {
    game_state.field_ec = 0;
    func_80127474();
    if (game_state.field_ec == 2)
        game_state.field_ee = 0;
}

void func_80127474(void) {
    if ((u8)func_80127cd8(1, -1, 0))
        game_state.field_ec |= 2;
    func_80137220(1, 0);
}

void func_801274c8(void) {
    game_state.field_ec = 0;
    func_80127510();
    if (game_state.field_ec == 4)
        game_state.field_ee = 0;
}

void func_80127510(void) {
    if ((u8)func_80127cd8(2, -1, 0))
        game_state.field_ec |= 4;
    func_80137220(2, 1);
}

void func_80127564(void) {
    game_state.field_ec = 0;
    func_801275ac();
    if (game_state.field_ec == 8)
        game_state.field_ee = 0;
}

void func_801275ac(void) {
    if ((u8)func_80127cd8(3, -1, 0))
        game_state.field_ec |= 8;
    func_80137220(3, 2);
}

void func_80127600(void) {
    game_state.field_ec = 0;
    func_80127648();
    if (game_state.field_ec == 0x10)
        game_state.field_ee = 0;
}

void func_80127648(void) {
    if ((u8)func_80127cd8(4, -1, 0))
        game_state.field_ec |= 0x10;
    func_80137220(4, 7);
}

void func_8012769c(void) {
    game_state.field_ec = 0;
    func_8012774c();
    func_801277e8();
    func_80127884();
    func_80127920();
    func_801279bc();
    if (game_state.field_ec == 0x1f)
        game_state.field_ee = 0;
}

void func_80127704(void) {
    game_state.field_ec = 0;
    func_8012774c();
    if (game_state.field_ec == 1)
        game_state.field_ee = 0;
}

void func_8012774c(void) {
    if ((u8)func_80127cd8(0, 1, 1))
        game_state.field_ec |= 1;
    func_80137220(0, 6);
}

void func_801277a0(void) {
    game_state.field_ec = 0;
    func_801277e8();
    if (game_state.field_ec == 2)
        game_state.field_ee = 0;
}

void func_801277e8(void) {
    if ((u8)func_80127cd8(1, 1, 1))
        game_state.field_ec |= 2;
    func_80137220(1, 0);
}

void func_8012783c(void) {
    game_state.field_ec = 0;
    func_80127884();
    if (game_state.field_ec == 4)
        game_state.field_ee = 0;
}

void func_80127884(void) {
    if ((u8)func_80127cd8(2, 1, 1))
        game_state.field_ec |= 4;
    func_80137220(2, 1);
}

void func_801278d8(void) {
    game_state.field_ec = 0;
    func_80127920();
    if (game_state.field_ec == 8)
        game_state.field_ee = 0;
}

void func_80127920(void) {
    if ((u8)func_80127cd8(3, 1, 1))
        game_state.field_ec |= 8;
    func_80137220(3, 2);
}

void func_80127974(void) {
    game_state.field_ec = 0;
    func_801279bc();
    if (game_state.field_ec == 0x10)
        game_state.field_ee = 0;
}

void func_801279bc(void) {
    if ((u8)func_80127cd8(4, 1, 1))
        game_state.field_ec |= 0x10;
    func_80137220(4, 7);
}

void func_80127a10(void) {
    data_8018f5a0->field_6a--;
    if (data_8018f5a0->field_6a & 0x80) {
        game_state.field_ee = 0;
    } else {
        func_80127cd8(0, 1, 2);
        func_80127cd8(1, 1, 2);
        func_80127cd8(2, 1, 2);
        func_80127cd8(3, 1, 2);
        func_80127cd8(4, 1, 2);
        func_80137b10();
    }
}

void func_80127ac4(void) {
    data_8018f5a0->field_6a--;
    if (data_8018f5a0->field_6a & 0x80) {
        game_state.field_ee = 0;
    } else {
        func_80127cd8(0, -1, 3);
        func_80127cd8(1, -1, 3);
        func_80127cd8(2, -1, 3);
        func_80127cd8(3, -1, 3);
        func_80127cd8(4, -1, 3);
        func_80137b10();
    }
}

void func_80127b78(void) {
    data_8018f5a0->field_6a--;
    if (data_8018f5a0->field_6a & 0x80) {
        game_state.field_ee = 0;
    } else {
        func_80127cd8(0, 1, 2);
        func_80127cd8(2, 1, 2);
        func_80127cd8(3, 1, 2);
        func_80137220(0, 6);
        func_80137220(2, 1);
        func_80137220(3, 2);
    }
}

void func_80127c28(void) {
    data_8018f5a0->field_6a--;
    if (data_8018f5a0->field_6a & 0x80) {
        game_state.field_ee = 0;
    } else {
        func_80127cd8(0, -1, 3);
        func_80127cd8(2, -1, 3);
        func_80127cd8(3, -1, 3);
        func_80137220(0, 6);
        func_80137220(2, 1);
        func_80137220(3, 2);
    }
}
