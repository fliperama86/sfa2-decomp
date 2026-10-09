/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u8 data_80190464;
extern s16 data_8019046c;

int func_80141908(Object *object) {
    if (*(u16 *)&object->field_04 != 1) return 0;
    if (object->field_7e != 0) {
        if (object->field_17b != 0) return 0;
    } else if (object->field_06 == 7) {
        return 0;
    }
    if (object->field_06 == 8) return 0;
    if (object->field_06 == 9) return 0;
    if (object->field_06 == 5) {
        if (data_8019046c != 0) {
            if (object->field_d8 != 0) return 0;
            if (object->field_29b != 0) goto go;
        }
        if (object->field_248 == 0 && object->field_49 == 0) return 0;
    }
go:
    if (object->field_163 != 0) return 0;
    return func_80141a00(object) & 0xff;
}

int func_80141a00(Object *object) {
    object->field_69 = 0;
    object->field_6a = 0;
    if (game_state.field_30 != 0 && game_state.mode != object->side + 1) {
        object->field_19f = 0;
        object->field_1a2 = 0;
    }
    if (object->field_29a != 0) return 0;
    if (object->field_7e == 0) {
        *(u8 *)&game_state.field_354 = game_state.config->field_4d;
        *(u8 *)&game_state.field_354 |= game_state.config->field_4e;
        *(u8 *)&game_state.field_354 |= game_state.config->field_04;
        if (*(u8 *)&game_state.field_354 != 0) return 0;
        if ((func_8012f56c(object) & 0xff) != 0) return 0;
    }
    if (data_80190464 == 0) {
        object->field_bd = 2;
        if (object->field_cd == 0) func_80155d4c(0xd, (s8)object->side);
    }
    object->field_4b = object->field_25c;
    object->field_157 = 0;
    func_80142c04(object);
    return 1;
}

int func_80141b28(Object *object) {
    if ((s16)object->field_c6 < 0x30) return 0;
    if (*(u16 *)&object->field_04 != 0x101) return 0;
    if (object->field_61 != 0xff) return 0;
    if (object->field_45 != 0) return 0;
    if ((game_state.config->field_4d | game_state.config->field_4e | game_state.config->field_04) != 0) return 0;
    if ((func_8012f56c(object) & 0xff) != 0) return 0;
    if ((s16)object->field_46 >= 8) return 0;
    func_80141f28(object, -0x30);
    object->field_bd = 4;
    func_80138b94(&game_state, object);
    set_side_field_0d(object);
    if (object->field_cd == 0) func_80155d4c(0xd, (s8)object->side);
    func_8012f5ec(object);
    object->field_157 = 0;
    object->field_4b = object->field_4b;
    func_80142c04(object);
    return 1;
}
