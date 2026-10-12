/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801449fc(Object *object) {
    object->state_0++;
}

void func_80144a10(Object *object) {
    table_8017b654[object->state_2](object);
    advance_sequence_checked(object);
    func_80120028(object);
}

void func_80144a6c(Object *object) {
    object->pos_x = 0x1c0;
    object->pos_y = 0x70;
    object->countdown = 0xf;
    object->vel_x = 0xfff40000;
    object->accel_x = -0x8000;
    object->state_2++;
    func_80144ee4(object, 9);
}

void func_80144ac0(Object *object) {
    int v;
    func_80144220(object);
    v = object->countdown - 1;
    object->countdown = v;
    if (v & 0x80) {
        object->pos_x = 0xc0;
        object->state_2++;
    }
}

void func_80144b14(Object *object) {
    if ((s16)object->step_flags < 0) {
        if ((u8)func_801441c8(object) != 0 || game_state.config->table_index == 0) {
            func_801449fc(object);
        } else {
            object->state_2 = 3;
            func_80144ee4(object, 0xe);
        }
    }
}
