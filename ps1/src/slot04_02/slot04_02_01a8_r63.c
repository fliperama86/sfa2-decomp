/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c77d8_slot04_02[];
extern ObjectFn data_801c77ec_slot04_02[];
void func_801b7908_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);
void func_801b7920_slot04_02(Object *obj, u8 a);

void func_801b7110_slot04_02(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    if (o->field_03 >= 6) {
        o->field_af ^= 1;
        o->field_0d = o->field_af + obj->field_b0;
    }
    data_801c77d8_slot04_02[o->state_1](o);
}

void func_801b717c_slot04_02(Object *o) {
    if ((game_state.field_65 | game_state.field_a8) == 0) {
        data_801c77ec_slot04_02[o->field_03 >> 1](o);
        advance_sequence_checked(o);
        func_8011ff74(o);
    }
    func_8011ffdc(o);
}

void func_801b71fc_slot04_02(Object *o) {
    *(s32 *)&o->pos_x_frac += o->vel_x;
    *(s32 *)&o->pos_y_frac -= o->vel_y;
}

void func_801b7220_slot04_02(Object *obj) {
    if (obj->pos_y >= (s16)(obj->ground_y - 0x10)) {
        func_801b7908_slot04_02(obj, 2, 0, 0, 0);
        obj->pos_y = obj->ground_y;
        func_801b7920_slot04_02(obj, 0x10);
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    }
}
