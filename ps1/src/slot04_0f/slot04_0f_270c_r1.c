/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c5b08_slot04_0f[];
extern ObjectFn data_801c5b18_slot04_0f[];

void func_80138b38(GameState *state, Object *object);
void func_80138ae8(GameState *state, Object *object);
void func_80142adc(Object *object);
void func_80142fe8(Object *object);
void func_80142fbc(Object *object);
void func_801428a8(Object *object);
void func_80142c70(Object *object);
void func_80130678(Object *object, int arg);
void func_801495f8(Object *object, int x, int y);
void func_8011fe40(Object *object);
void func_80149af8(Object *object);
void func_80143184(Object *object);
void func_801b2ec4_slot04_0f(Object *obj);
void func_801b470c_slot04_0f(Object *obj);

void func_801b270c_slot04_0f(Object *obj) {
    int a;

    obj->field_17b = 1;
    obj->field_07++;
    func_80141f28(obj, 6);
    func_80138ae8((GameState *)game_state.config, obj);
    a = 0x3e;
    if (obj->field_49 != 0) {
        a = 0x5a;
    }
    func_801307e0(obj, a + (obj->field_12a >> 1));
}

void func_801b2784_slot04_0f(Object *obj) {
    Object *p;
    s32 y;

    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07++;
        p = func_8011f0e8(obj);
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x19;
            p->field_03 = 0;
            p->field_66 = obj->field_66;
            p->field_65 = obj->field_65;
            p->field_ac = obj->field_12a;
            p->field_ad = 0;
            p->field_0e = obj->field_0e;
            p->field_0b = obj->field_0b;
            p->field_0c = obj->field_0c;
            p->field_26 = obj->field_26;
            *(s32 *)&p->field_10 = *(s32 *)&obj->field_10;
            y = *(s32 *)&obj->field_14;
            p->field_7a = 0x60;
            p->field_5c = 0;
            p->field_3c = obj;
            p->field_7c = 0x1e0;
            *(s32 *)&p->field_14 = y;
            p->field_0d = obj->field_0d;
            p->field_90 = obj->field_90;
            p->field_98 = obj->field_98;
            p->field_9c = obj->field_9c;
            obj->field_14c = (s32)p;
            obj->field_240++;
            func_801204f4(obj, obj->side, 0x11);
        }
    }
}

void func_801b28b8_slot04_0f(Object *obj) {
    if (obj->field_14c == 0) {
        obj->field_17b = 0;
        obj->field_07++;
        func_801307e0(obj, 0x44);
    } else {
        func_80130efc(obj);
    }
}

void func_801b2904_slot04_0f(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        func_801b470c_slot04_0f(obj);
    } else {
        func_80142adc(obj);
        func_80130efc(obj);
    }
}

void func_801b2958_slot04_0f(Object *obj) {
    data_801c5b08_slot04_0f[obj->field_07](obj);
}

void func_801b2998_slot04_0f(Object *obj) {
    int a;

    obj->field_17b = 1;
    obj->field_07++;
    func_80141f28(obj, 6);
    func_80138ae8((GameState *)game_state.config, obj);
    a = 0x41;
    if (obj->field_49 != 0) {
        a = 0x5d;
    }
    func_801307e0(obj, a + (obj->field_12a >> 1));
}

void func_801b2a10_slot04_0f(Object *obj) {
    Object *p;
    s32 y;

    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07++;
        p = func_8011f0e8(obj);
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x19;
            p->field_03 = 2;
            p->field_66 = obj->field_66;
            p->field_65 = obj->field_65;
            p->field_ac = obj->field_12a + 6;
            p->field_ad = 0;
            p->field_0e = obj->field_0e;
            p->field_0b = obj->field_0b;
            p->field_0c = obj->field_0c;
            p->field_26 = obj->field_26;
            *(s32 *)&p->field_10 = *(s32 *)&obj->field_10;
            y = *(s32 *)&obj->field_14;
            p->field_7a = 0x60;
            p->field_5c = 0;
            p->field_3c = obj;
            p->field_7c = 0x1e0;
            *(s32 *)&p->field_14 = y;
            p->field_0d = obj->field_0d;
            p->field_90 = obj->field_90;
            p->field_98 = obj->field_98;
            p->field_9c = obj->field_9c;
            obj->field_254 = 1;
            obj->field_14c = (s32)p;
            obj->field_46 = 3;
            obj->field_240++;
        }
        func_801204f4(obj, obj->side, 0x11);
    }
}

void func_801b2b5c_slot04_0f(Object *obj) {
    if (obj->field_14c == 0) {
        obj->field_07++;
        func_801307e0(obj, 0x45);
    } else {
        func_80130efc(obj);
    }
}

void func_801b2ba8_slot04_0f(Object *obj) {
    s16 t;

    if ((s16)obj->field_3a < 0) {
        func_801b470c_slot04_0f(obj);
    } else {
        t = obj->field_46;
        if (t != 0) {
            t = t - 1;
            obj->field_46 = t;
            if (t == 0) {
                goto tail;
            }
            obj->field_17b = 0;
        }
        func_80142adc(obj);
    tail:
        func_80130efc(obj);
    }
}

void func_801b2c24_slot04_0f(Object *obj) {
    data_801c5b18_slot04_0f[obj->field_07](obj);
}

void func_801b2c64_slot04_0f(Object *obj) {
    int a;
    obj->field_07 = obj->field_07 + 1;
    func_80142fbc(obj);
    func_801428a8(obj);
    func_80138b38((GameState *)game_state.config, obj);
    if (obj->field_45 != 0) {
        a = 0x2f;
    } else {
        a = 0x2b;
    }
    func_80130678(obj, a);
}

void func_801b2cd0_slot04_0f(Object *obj) {
    int a = 0;
    int b = 0x55;
    if ((s16)obj->field_3a & 0xff00) {
        obj->field_165 = 0xff;
        obj->field_46 = 0x1e01;
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_45 == 0) {
            a = 2;
        }
        func_801495f8(obj, a, b);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b2ec4_slot04_0f(obj);
    }
    func_80130efc(obj);
}

void func_801b2d5c_slot04_0f(Object *obj) {
    int a;
    ((Slot04bObj *)obj)->field_47--;
    if (((Slot04bObj *)obj)->field_47 == 0) {
        a = ((Slot04bObj *)obj)->field_c6;
        obj->field_07++;
        ((Slot04bObj *)obj)->field_47 = 0x14;
        obj->field_27c = 0;
        ((Slot04bObj *)obj)->field_27d = 0;
        a += 0x1e;
        if (obj->field_4b != 0) {
            a = 0x48;
        }
        obj->field_2a1 = a;
        obj->field_180 = 0;
    }
    func_80142fe8(obj);
    func_80130efc(obj);
}

void func_801b2de4_slot04_0f(Object *obj) {
    obj->field_27c++;
    func_80130efc(obj);
    ((Slot04bObj *)obj)->field_46--;
    if (((Slot04bObj *)obj)->field_46 == 0) {
        if (obj->field_45 != 0) {
            obj->field_04 = 1;
            obj->field_05 = 0;
            obj->field_06 = 3;
            obj->field_07 = 1;
        }
        func_80142c70(obj);
    } else {
        if (obj->field_134 != 0) {
            if (((Slot04bObj *)obj)->field_27d == 0) {
                ((Slot04bObj *)obj)->field_27d = obj->field_27c;
            }
        } else if (((Slot04bObj *)obj)->field_47 != 0) {
            ((Slot04bObj *)obj)->field_47--;
        }
        func_80142fe8(obj);
    }
}
