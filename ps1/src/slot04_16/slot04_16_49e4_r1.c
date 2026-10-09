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
        obj->field_07++;
        obj->pos_y = obj->field_70;
        *(s32 *)&obj->field_14 &= 0xffff0000;
        func_801307e0(obj, 0x28);
    }
}

void func_801b4a54_slot04_16(Object *obj) {
    if ((s16)obj->field_3a < 0) {
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
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07 = 8;
    }
    func_80130efc(obj);
}

void func_801b4b08_slot04_16(Object *obj) {
    s32 d = obj->field_4c;

    if (obj->field_0b == 0) {
        d = -d;
    }
    *(s32 *)&obj->field_10 += d;
    obj->field_4c += obj->field_54;
    if (obj->field_4c < 0) {
        obj->field_54 = 0;
        obj->field_4c = 0;
    }
}

void func_801b4b50_slot04_16(Object *obj) {
    data_801ca384_slot04_16[obj->field_07](obj);
}

void func_801b4b90_slot04_16(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    obj->field_298 = 0;
    o->field_07++;
    func_801428e4(o);
    func_80138b38((GameState *)game_state.config, o);
    func_80145d20(o);
    o->field_4c = data_801ca3cc_slot04_16[o->field_12a * 2];
    o->field_50 = data_801ca3cc_slot04_16[o->field_12a * 2 + 1];
    o->field_54 = data_801ca3cc_slot04_16[o->field_12a * 2 + 2];
    o->field_58 = data_801ca3cc_slot04_16[o->field_12a * 2 + 3];
    if (o->field_0b == 0) {
        o->field_4c = -o->field_4c;
        o->field_54 = -o->field_54;
    }
    func_801307e0(o, 0x30);
}

void func_801b4c90_slot04_16(Object *obj) {
    if ((s16)obj->field_3a & 0xff00) {
        obj->field_07++;
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
