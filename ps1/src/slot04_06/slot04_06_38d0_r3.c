/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4850_slot04_06(Object *object);

void func_801b3b70_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    func_801b4850_slot04_06(o);
    if (o->pos_y >= o->field_70) {
        o->state_3++;
        func_80130af0(o);
        o->field_6a = 0;
        o->field_69 = 0;
        o->field_45 = 0;
        o->field_259 = 0;
        o->field_258 = 0;
        o->field_25a = 0;
        func_8013786c(o);
        o->pos_y = (u16)o->field_70;
        obj->field_1c2 = 0xa;
        game_state.field_63 = 0xa;
        play_side_sound_at(o, o->side, 0xa);
    } else {
        *(s32 *)&o->pos_x_frac += o->vel_x;
        o->vel_x += o->accel_x;
    }
}

void func_801b3c34_slot04_06(Object *obj) {
    obj->field_12c = 0;
    obj->field_12d = 0;
    obj->field_12e = 0;
    obj->field_12f = 0;
    obj->state_3++;
    func_80141f28(obj, 7);
    func_80138ae8(&game_state, obj);
    obj->field_0b = obj->field_158;
    func_801307e0(obj, 0x31);
}

void func_801b3ca0_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    if ((s16)o->step_flags >= 0) {
        update_sequence(o);
    } else {
        obj->field_1c6 = 0x1e;
        o->field_159 = 1;
        o->state_3++;
        func_801307e0(o, 0x32);
    }
}
