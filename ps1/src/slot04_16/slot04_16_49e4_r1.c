/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ca384_slot04_16[];
extern s32 data_801ca3cc_slot04_16[];

int func_80130184(Object *object);
void func_80142adc(Object *object);
void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80145d20(Object *object);
void func_801483a4(Object *object, int a, int b);
void func_801b10b0_slot04_16(Object *object);
void func_801b4b08_slot04_16(Object *object);

void func_801b49e4_slot04_16(Object *obj) {
    if ((u8)func_80130184(obj) != 0) {
        func_80130efc(obj);
    } else {
        obj->state_3++;
        obj->pos_y = obj->field_70;
        *(s32 *)&obj->pos_y_frac &= 0xffff0000;
        func_801307e0(obj, 0x28);
    }
}

void func_801b4a54_slot04_16(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->field_17b = 0;
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b10b0_slot04_16(obj);
    } else {
        func_80142adc(obj);
        func_80130efc(obj);
    }
}

void func_801b4ac4_slot04_16(Object *obj) {
    func_801b4b08_slot04_16(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = 8;
    }
    func_80130efc(obj);
}

void func_801b4b08_slot04_16(Object *obj) {
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

void func_801b4b50_slot04_16(Object *obj) {
    data_801ca384_slot04_16[obj->state_3](obj);
}

void func_801b4b90_slot04_16(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    obj->field_298 = 0;
    o->state_3++;
    func_801428e4(o);
    func_80138b38((GameState *)game_state.config, o);
    func_80145d20(o);
    o->vel_x = data_801ca3cc_slot04_16[o->field_12a * 2];
    o->vel_y = data_801ca3cc_slot04_16[o->field_12a * 2 + 1];
    o->accel_x = data_801ca3cc_slot04_16[o->field_12a * 2 + 2];
    o->accel_y = data_801ca3cc_slot04_16[o->field_12a * 2 + 3];
    if (o->field_0b == 0) {
        o->vel_x = -o->vel_x;
        o->accel_x = -o->accel_x;
    }
    func_801307e0(o, 0x30);
}

void func_801b4c90_slot04_16(Object *obj) {
    if ((s16)obj->step_flags & 0xff00) {
        obj->state_3++;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        func_801483a4(obj, -0x24, 0x2b);
        func_80120554(obj, obj->side, 0x31c);
    }
    func_80130efc(obj);
}
