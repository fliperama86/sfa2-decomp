/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c5ac0_slot04_0f[];
extern ObjectFn data_801c5ad4_slot04_0f[];

void set_sequence_by_side(Object *object, int index);

void func_801b1fd8_slot04_0f(Object *obj);
void func_801b1ddc_slot04_0f(Object *obj);
void func_801b2094_slot04_0f(Object *obj);
void func_801b21f8_slot04_0f(Object *o, int a, int pos);
void func_801b2270_slot04_0f(Object *obj);

void func_801b1d04_slot04_0f(Object *obj) {
    if (obj->field_49 != 0) {
        func_801b1fd8_slot04_0f(obj);
    } else {
        data_801c5ac0_slot04_0f[obj->state_3](obj);
    }
}

void func_801b1d64_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    o->state_3 = o->state_3 + 1;
    obj->field_334 = 1;
    func_80141f28(o, 2);
    *(s32 *)&obj->field_330 = *(s32 *)&o->other->pos_x_frac;
    obj->field_339 = 0xff;
    func_801307e0(o, 0x39);
    play_side_sound_at(o, o->side, 0xe);
    func_801b1ddc_slot04_0f(o);
}

/* The call of func_801b21f8_slot04_0f passes one argument although the callee takes three: the original does not set the second and third argument registers before it. Written with the arguments (two zeros), this unit is 8 bytes longer and differs from the original in 10 instruction slots. */
void func_801b1ddc_slot04_0f(Object *o) {
    update_sequence(o);
    func_801b2270_slot04_0f(o);
    if ((s16)o->step_flags < 0) {
        o->state_3 = o->state_3 + 1;
        o->field_01 = 0;
        ((Slot04bObj *)o)->field_46 = 0x10;
        ((void (*)(Object *))func_801b21f8_slot04_0f)(o);
    }
}

void func_801b1e38_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    obj->field_46--;
    if (obj->field_46 == 0) {
        o->field_01 = 1;
        o->state_3 = o->state_3 + 1;
        o->field_0b = o->field_158;
        func_801307e0(o, 0x3a);
        play_side_sound_at(o, o->side, 0xf);
    } else {
        *(s32 *)&o->pos_x_frac = *(s32 *)&o->pos_x_frac + o->vel_x;
    }
}

void func_801b1ec8_slot04_0f(Object *o) {
    update_sequence(o);
    func_801b2270_slot04_0f(o);
    if ((s16)o->step_flags < 0) {
        ((Slot04bObj *)o)->field_46 = 1;
        o->state_3 = o->state_3 + 1;
        if (o->field_45 == 0) {
            o->field_46 = (o->field_46 & 0xff00) | 6;
            set_sequence_by_side(o, 0);
        }
    }
}

void func_801b1f3c_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    update_sequence(o);
    obj->field_46--;
    if (obj->field_46 == 0) {
        if (o->field_45 != 0) {
            o->state_0 = 1;
            o->state_1 = 0;
            o->state_2 = 3;
            o->state_3 = 1;
            o->vel_y = 0;
            o->accel_x = 0;
            o->vel_x = 0;
            o->accel_y = -0x3000;
            set_sequence_by_side(o, 0x21);
        } else {
            func_801312b8(o);
        }
    }
}

void func_801b1fd8_slot04_0f(Object *obj) {
    data_801c5ad4_slot04_0f[obj->state_3](obj);
}

void func_801b2018_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 one = 1;

    o->state_3 = o->state_3 + 1;
    obj->field_334 = one;
    func_80141f28(o, 2);
    o->field_17b = one;
    *(s32 *)&obj->field_330 = *(s32 *)&o->other->pos_x_frac;
    func_801307e0(o, 0x55);
    play_side_sound_at(o, o->side, 0xe);
    func_801b2094_slot04_0f(o);
}

/* The call of func_801b21f8_slot04_0f passes one argument although the callee takes three: the original does not set the second and third argument registers before it. Written with the arguments (two zeros), this unit is 8 bytes longer and differs from the original in 6 instruction slots. */
void func_801b2094_slot04_0f(Object *o) {
    update_sequence(o);
    if ((s16)o->step_flags < 0) {
        ((Slot04bObj *)o)->field_46 = 0x10;
        o->state_3 = o->state_3 + 1;
        ((void (*)(Object *))func_801b21f8_slot04_0f)(o);
    }
}

void func_801b20e4_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    obj->field_46--;
    if (obj->field_46 == 0) {
        o->state_3 = o->state_3 + 1;
        func_801307e0(o, 0x56);
        play_side_sound_at(o, o->side, 0xf);
    } else {
        *(s32 *)&o->pos_x_frac = *(s32 *)&o->pos_x_frac + o->vel_x;
    }
}

void func_801b2168_slot04_0f(Object *o) {
    update_sequence(o);
    if ((s16)o->step_flags < 0) {
        o->field_17b = 0;
        if (o->field_45 == 0) {
            func_801312b8(o);
        } else {
            o->state_0 = 1;
            o->state_2 = 3;
            o->state_3 = 3;
            o->state_1 = 0;
            o->vel_y = 0;
            o->accel_x = 0;
            o->vel_x = 0;
            o->accel_y = -0x6000;
            set_sequence_by_side(o, 0x21);
        }
    }
}
