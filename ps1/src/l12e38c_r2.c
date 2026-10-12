/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8012ec50(Object *object) {
    if ((u8)func_80130184(object) == 0) {
        s32 speed;
        int arg;

        object->state_2 = 2;
        object->field_249 = 5;
        object->field_45 = 0;
        object->field_159 = 0;
        object->field_6a = 0;
        object->pos_y = object->ground_y;
        *(u32 *)&object->pos_y_frac &= 0xffff0000;
        object->field_0b = object->field_158;
        if (game_state.field_30 != 0 && game_state.mode != object->side + 1) {
            u8 count;

            count = object->field_1a1 - 1;
            object->field_1a1 = count;
            if (count & 0x80) {
                object->field_19f = 0;
            }
        }
        object->field_69 = 0;
        object->field_258 = 0;
        object->field_259 = 0;
        object->field_25a = 0;
        object->field_46 = *(u16 *)(data_80171a9c + (object->field_12a & 0xfe));
        set_side_flag(object);
        func_80130af0(object);
        func_801209c4(object);
        speed = data_80171aa4[object->kind];
        arg = 0x1e;
        if (object->field_0b != 0) {
            speed = -speed;
        }
        if (object->field_25b != 0) {
            speed = -speed;
            arg = 0x1f;
        }
        object->vel_x = speed;
        func_80130678(object, arg);
    } else {
        update_sequence(object);
        func_801376b8(object);
    }
}
