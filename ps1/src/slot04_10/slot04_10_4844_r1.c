/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c6bb8_slot04_10[];
extern s32 data_801c6bc0_slot04_10[];
extern ObjectFn data_801c6bf0_slot04_10[];
extern s32 data_801c6c38_slot04_10[];

int func_80130184(Object *object);
void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80142adc(Object *object);
void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80145d20(Object *object);
void func_801483a4(Object *object, int a, int b);
void func_801b1078_slot04_10(Object *object);
void func_801b4ad0_slot04_10(Object *object);

void func_801b4844_slot04_10(Object *obj) {
    s32 a;

    if (*(u8 *)&obj->step_flags == 0) {
        update_sequence(obj);
    } else {
        obj->field_45 = 1;
        obj->state_3++;
        obj->step_flags &= 0xff00;
        ref_other.p = obj->other;
        ref_other.p->field_15b = 1;
        func_80140770(obj, data_801c6bb8_slot04_10[obj->field_12a & 0xfe], 0xf, -0x200, 0, 0, 1);
        ref_other.p = obj->other;
        if (((Slot04bObj *)ref_other.p)->field_5c < 0) {
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x20;
                obj->field_255 = 6;
            }
        }
        a = data_801c6bc0_slot04_10[obj->field_12a * 2];
        obj->vel_y = data_801c6bc0_slot04_10[obj->field_12a * 2 + 1];
        obj->accel_y = data_801c6bc0_slot04_10[obj->field_12a * 2 + 3];
        obj->accel_x = 0;
        if (obj->field_0b != 0) {
            a = -a;
        }
        obj->vel_x = a;
    }
}

void func_801b49ac_slot04_10(Object *obj) {
    if ((u8)func_80130184(obj) != 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->pos_y = obj->field_70;
        *(s32 *)&obj->pos_y_frac &= 0xffff0000;
        func_801307e0(obj, 0x28);
    }
}

void func_801b4a1c_slot04_10(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->field_17b = 0;
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b1078_slot04_10(obj);
    } else {
        func_80142adc(obj);
        update_sequence(obj);
    }
}

void func_801b4a8c_slot04_10(Object *obj) {
    func_801b4ad0_slot04_10(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = 8;
    }
    update_sequence(obj);
}

void func_801b4ad0_slot04_10(Object *obj) {
    s32 d = obj->vel_x;

    if (obj->field_0b == 0) {
        d = -d;
    }
    *(s32 *)&obj->pos_x_frac += d;
    obj->vel_x += obj->accel_x;
    if (obj->vel_x < 0) {
        obj->accel_x = 0;
        obj->vel_x = 0;
    }
}

void func_801b4b18_slot04_10(Object *obj) {
    data_801c6bf0_slot04_10[obj->state_3](obj);
}

void func_801b4b58_slot04_10(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    obj->field_298 = 0;
    o->state_3++;
    func_801428e4(o);
    func_80138b38((GameState *)game_state.config, o);
    func_80145d20(o);
    o->vel_x = data_801c6c38_slot04_10[o->field_12a * 2];
    o->vel_y = data_801c6c38_slot04_10[o->field_12a * 2 + 1];
    o->accel_x = data_801c6c38_slot04_10[o->field_12a * 2 + 2];
    o->accel_y = data_801c6c38_slot04_10[o->field_12a * 2 + 3];
    if (o->field_0b == 0) {
        o->vel_x = -o->vel_x;
        o->accel_x = -o->accel_x;
    }
    func_801307e0(o, 0x30);
}

void func_801b4c58_slot04_10(Object *obj) {
    if ((s16)obj->step_flags & 0xff00) {
        obj->state_3++;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        func_801483a4(obj, -0x24, 0x2b);
        play_sound_at(obj, obj->side, 0x31c);
    }
    update_sequence(obj);
}
