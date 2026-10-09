/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801417cc(Object *object);
void func_80142adc(Object *object);
void func_80142ba0(Object *object);
Pooled *func_8011f4a4(void);
void func_80138ae8(GameState *state, Object *object);
void func_801b1510_slot04_0c(Object *obj);

extern ObjectFnInt data_801bd8f8_slot04_0c[];
extern ObjectFn data_801bd92c_slot04_0c[];
extern ObjectFn data_801bd960_slot04_0c[];
extern ObjectFn data_801bd9c4_slot04_0c[];
extern u16 data_801bd974_slot04_0c[];
extern s32 data_801bd994_slot04_0c[];
extern u16 data_801a2944[];
extern u8 data_801ad398;

void func_801b0eb0_slot04_0c(Object *obj) {
    u8 t;

    t = 1;
    obj->field_04 = t;
    obj->field_159 = t;
    t = obj->field_158;
    obj->field_06 = 7;
    obj->field_15a = 5;
    obj->field_27b = 0x18;
    obj->field_0b = t;
    obj->field_05 = 0;
    obj->field_07 = 0;
    obj->field_157 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x14;
    func_801307e0(obj, 0x19);
}

int func_801b0f18_slot04_0c(Object *obj) {
    int r = 0;

    if (obj->field_240 == 0) {
        if (func_801417cc(obj) != 0) {
            r = 1;
            obj->field_04 = 1;
            obj->field_05 = 0;
            obj->field_06 = 7;
            obj->field_07 = 0;
            obj->field_15a = 1;
            obj->field_159 = 1;
            obj->field_0b = obj->field_158;
            func_80142b3c(obj);
        }
    }
    return r;
}

int func_801b0f9c_slot04_0c(Object *obj) {
    int r = 0;

    if (func_801417cc(obj) != 0) {
        r = 1;
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 0;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
        func_80142b3c(obj);
    }
    return r;
}

int func_801b1010_slot04_0c(Object *obj) {
    int r = 0;
    u8 one;

    if (func_801417cc(obj) != 0) {
        r = 1;
        one = 1;
        obj->field_04 = one;
        obj->field_05 = 0;
        obj->field_06 = 7;
        obj->field_07 = 0;
        obj->field_15a = 3;
        obj->field_159 = one;
        obj->field_0b = obj->field_158;
        func_80142ba0(obj);
    }
    return r;
}

void func_801b1088_slot04_0c(Object *obj) {
    data_801ad398 = data_801bd8f8_slot04_0c[obj->field_15a](obj);
}

int func_801b10d0_slot04_0c(Object *obj) {
    return obj->field_240 == 0;
}

int func_801b10dc_slot04_0c(Object *obj) {
    return 1;
}

int func_801b10e4_slot04_0c(Object *obj) {
    int r = 0;

    if (obj->field_240 == 0) {
        r = (s16)obj->field_c6 >= 0x30;
    }
    return r;
}

int func_801b110c_slot04_0c(Object *obj) {
    return (s16)obj->field_c6 >= 0x30;
}

void func_801b1120_slot04_0c(Object *obj) {
    data_801bd92c_slot04_0c[obj->field_15a](obj);
}

void func_801b1160_slot04_0c(Object *obj) {
    data_801bd960_slot04_0c[obj->field_07](obj);
}

void func_801b11a0_slot04_0c(Object *o) {
    int a = 0x1c;
    Job *job;
    u16 *dst;
    u16 *src;
    int i;
    u8 idx;

    o->field_17b = 1;
    o->field_07++;
    func_80141f28(o, 9);
    func_80138ae8(&game_state, o);
    if ((func_80151184() & 7) == 0) {
        o->field_27b = 0xff;
        if (o->field_49 == 0) {
            job = (Job *)func_8011f4a4();
            if (job != 0) {
                job->mode = 2;
                job->kind = 1;
                src = (u16 *)(job->src = (u8 *)data_801bd974_slot04_0c);
                job->rect.x = 0x60;
                job->rect.y = o->field_0d + 0x1e0;
                job->rect.w = 0x10;
                job->rect.h = 1;
                dst = data_801a2944;
                if (o->side == 0) {
                    dst += 0x50;
                }
                for (i = 0; i < 0x10; i++) {
                    *dst++ = *src++;
                }
            }
        }
    }
    idx = o->field_12a;
    o->field_4c = data_801bd994_slot04_0c[idx * 2];
    o->field_50 = data_801bd994_slot04_0c[idx * 2 + 1];
    o->field_54 = data_801bd994_slot04_0c[idx * 2 + 2];
    o->field_58 = data_801bd994_slot04_0c[idx * 2 + 3];
    if (o->field_49 != 0) {
        a = 0x30;
    }
    func_801307e0(o, (o->field_12a >> 1) + a);
    func_80120554(o, o->side, 0x320);
}

void func_801b134c_slot04_0c(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->field_3a == 0) {
        o->field_45 = 1;
        o->field_27b = 0;
        o->field_07++;
        func_8013786c(o);
    }
    func_80130efc(o);
}

void func_801b13a0_slot04_0c(Object *obj) {
    func_801b1510_slot04_0c(obj);
    if (obj->field_0b == 0) {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
    } else {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    }
    obj->field_4c = obj->field_4c + obj->field_54;
    if (obj->field_4c < 0) {
        obj->field_07++;
    }
    func_80130efc(obj);
}

void func_801b142c_slot04_0c(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    func_801b1510_slot04_0c(o);
    if (o->pos_y < o->field_70) {
        if (obj->field_3a != 0) {
            return;
        }
    } else {
        o->field_07++;
        o->field_45 = 0;
        o->field_159 = 0;
        o->field_17b = 0;
        o->pos_y = (u16)o->field_70;
        func_801209c4(o);
    }
    func_80130efc(o);
}

void func_801b14b0_slot04_0c(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801312b8(obj);
    } else {
        obj->other->field_249 = 5;
        func_80142adc(obj);
        func_80130efc(obj);
    }
}

void func_801b1510_slot04_0c(Object *obj) {
    if (obj->field_49 != 0 && obj->field_50 < 0) {
        obj->field_58 = 0xffff0000;
    }
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
}

void func_801b155c_slot04_0c(Object *obj) {
    data_801bd9c4_slot04_0c[obj->field_07](obj);
}

void func_801b159c_slot04_0c(Object *obj) {
    int a = 0x1f;

    obj->field_17b = 1;
    obj->field_07++;
    func_80141f28(obj, 5);
    func_80138ae8(&game_state, obj);
    if (obj->field_49 != 0) {
        a = 0x36;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + a);
}
